#include "abov/ll/gpio.h"
#include <stddef.h>
#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

/* a33g supports 15 pins per port */
#define MAX_PIN_NUMBER				15U
#define PIN_NUMBER_MASK				(ABOV_GPIO_PORT_SIZE - 1)
#define PER_BASE_POS				8U

ABOV_STATIC_ASSERT(ABOV_GPIO_PORT_SIZE == 0x100, "");

static uint32_t get_pin_from_gpio_number(uint32_t ngpio)
{
	return ngpio & PIN_NUMBER_MASK;
}

static abov_gpio_port_t get_port_from_gpio_number(uint32_t ngpio)
{
	return (abov_gpio_port_t)(ngpio & ~PIN_NUMBER_MASK);
}

static PCU_Type *get_pcu_from_gpio_number(uint32_t ngpio)
{
	switch (get_port_from_gpio_number(ngpio)) {
	case ABOV_GPIOA:
		return PCA;
	case ABOV_GPIOB:
		return PCB;
	case ABOV_GPIOC:
		return PCC;
	case ABOV_GPIOD:
		return PCD;
	case ABOV_GPIOE:
		return PCE;
	case ABOV_GPIOF:
		return PCF;
	default:
		return NULL;
	}
}

static GPIO_Type *get_reg_from_port(abov_gpio_port_t port)
{
	switch (port) {
	case ABOV_GPIOA:
		return PA;
	case ABOV_GPIOB:
		return PB;
	case ABOV_GPIOC:
		return PC;
	case ABOV_GPIOD:
		return PD;
	case ABOV_GPIOE:
		return PE;
	case ABOV_GPIOF:
		return PF;
	default:
		return NULL;
	}
}

static GPIO_Type *get_reg_from_gpio_number(uint32_t ngpio)
{
	return get_reg_from_port(get_port_from_gpio_number(ngpio));
}

static void enable_port(abov_gpio_port_t port)
{
	uint32_t pos = port >> 8;
	bitop_set(&PMU->PER, pos + PER_BASE_POS);
}

static void disable_port(abov_gpio_port_t port)
{
	uint32_t pos = port >> 8;
	bitop_clear(&PMU->PER, pos + PER_BASE_POS);
}

static void set_gpio_mode(PCU_Type *ctrl, uint32_t pin, abov_gpio_mode_t mode)
{
	uint32_t pos = pin * 2;
	uint32_t mask = 3U << pos;
	uint32_t val = 0;

	switch (mode) {
	case ABOV_GPIO_MODE_OPENDRAIN:
		val = 1;
		break;
	case ABOV_GPIO_MODE_INPUT:
	case ABOV_GPIO_MODE_INPUT_PULLUP:
	case ABOV_GPIO_MODE_INPUT_PULLDOWN:
		val = 2;
		break;
	case ABOV_GPIO_MODE_ANALOG:
		val = 3;
		break;
	case ABOV_GPIO_MODE_PUSHPULL:
	default:
		break;
	}

	bitop_clean_set_with_mask(&ctrl->CR, pos, mask, val);
}

static void set_gpio_pullmode(PCU_Type *ctrl, uint32_t pin, abov_gpio_mode_t mode)
{
	uint32_t reg = ctrl->PCR;

	bitop_clear(&reg, pin);
	bitop_clear(&reg, pin + 16);

	if (mode == ABOV_GPIO_MODE_INPUT_PULLDOWN) {
		bitop_set(&reg, pin + 16);
		bitop_set(&reg, pin);
	} else if (mode == ABOV_GPIO_MODE_INPUT_PULLUP) {
		bitop_set(&reg, pin);
	}

	ctrl->PCR = reg;
}

static void set_gpio_alt(PCU_Type *ctrl, uint32_t pin, int altfunc)
{
	uint32_t pos = pin * 2;
	bitop_clean_set_with_mask(&ctrl->MR, pos, 3U << pos, (uint32_t)altfunc);
}

static void disable_gpio_intr(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->IER &= ~(3U << (pin * 2));
}

static void disable_gpio_debounce(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->DER &= ~(3U << pin);
}

static bool set_gpio(uint32_t ngpio, abov_gpio_mode_t mode)
{
	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (ctrl == NULL || pin > MAX_PIN_NUMBER) {
		return false;
	}

	enable_port(get_port_from_gpio_number(ngpio));

	set_gpio_mode(ctrl, pin, mode);
	set_gpio_pullmode(ctrl, pin, mode);
	set_gpio_alt(ctrl, pin, 0);
	disable_gpio_intr(ctrl, pin);
	disable_gpio_debounce(ctrl, pin);

	return true;
}

bool abov_gpio_set_altfunc(uint32_t ngpio, int altfunc)
{
	if (altfunc > 3) {
		return false;
	}

	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	if (ctrl == NULL) {
		return false;
	}

	uint32_t pin = get_pin_from_gpio_number(ngpio);
	if (pin > MAX_PIN_NUMBER) {
		return false;
	}

	set_gpio_alt(ctrl, pin, altfunc);

	return true;
}

bool abov_gpio_open(uint32_t ngpio, abov_gpio_mode_t mode)
{
	return set_gpio(ngpio, mode);
}

bool abov_gpio_close(uint32_t ngpio)
{
	return set_gpio(ngpio, ABOV_GPIO_MODE_ANALOG);
}

bool abov_gpio_enable_intr(uint32_t ngpio, abov_gpio_intr_t intr_type)
{
	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (ctrl == NULL || pin > MAX_PIN_NUMBER) {
		return false;
	}

	uint32_t val = 0;
	bool edge = false;
	switch (intr_type) {
	case ABOV_GPIO_INTR_EDGE_RISING:
		edge = true;
		/* fall through */
	case ABOV_GPIO_INTR_LEVEL_HIGH:
		val = 2;
		break;
	case ABOV_GPIO_INTR_EDGE_FALLING:
		edge = true;
		/* fall through */
	case ABOV_GPIO_INTR_LEVEL_LOW:
		val = 1;
		break;
	case ABOV_GPIO_INTR_EDGE_ANY:
		edge = true;
		val = 3;
		break;
	default:
		break;
	}

	uint32_t pos = pin * 2;
	bitop_clean_set_with_mask(&ctrl->ICR, pos, 3U << pos, val);
	bitop_clean_set_with_mask(&ctrl->IER, pos, 3U << pos, ((uint32_t)edge << 1) + 1);

	return true;
}

bool abov_gpio_disable_intr(uint32_t ngpio)
{
	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (ctrl == NULL || pin > MAX_PIN_NUMBER) {
		return false;
	}

	ctrl->IER &= ~(3U << (pin * 2));

	return true;
}

void abov_gpio_clear_intr_flag(uint32_t ngpio)
{
	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (ctrl == NULL || pin > MAX_PIN_NUMBER) {
		return;
	}

	ctrl->ISR |= 1U << (pin * 2);
}

void abov_gpio_write(uint32_t ngpio, int value)
{
	GPIO_Type *reg = get_reg_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (reg == NULL || pin > MAX_PIN_NUMBER) {
		return;
	}

	reg->SRR = 1U << (pin + ((uint32_t)!(value & 1) * 16));
}

int abov_gpio_read(uint32_t ngpio)
{
	const GPIO_Type *reg = get_reg_from_gpio_number(ngpio);
	uint32_t pin = get_pin_from_gpio_number(ngpio);

	if (reg == NULL || pin > MAX_PIN_NUMBER) {
		return -1;
	}

	return (reg->IDR >> pin) & 0x1;
}

void abov_gpio_write_port(abov_gpio_port_t port, int value)
{
	GPIO_Type *reg = get_reg_from_port(port);

	if (reg != NULL) {
		reg->ODR = (uint32_t)value;
	}
}

int abov_gpio_read_port(abov_gpio_port_t port)
{
	const GPIO_Type *reg = get_reg_from_port(port);

	if (reg == NULL) {
		return -1;
	}

	return (int)reg->IDR;
}

void abov_gpio_enable_port(abov_gpio_port_t port)
{
	enable_port(port);
}

void abov_gpio_disable_port(abov_gpio_port_t port)
{
	disable_port(port);
}

void abov_gpio_reset(abov_gpio_port_t port)
{
	unused(port);
}
