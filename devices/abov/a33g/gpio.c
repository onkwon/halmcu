#include "abov/ll/gpio.h"

#include <stddef.h>
#include <stdbool.h>
#include <assert.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

/* a33g supports 15 pins per port */
#define MAX_PIN_NUMBER				16U
#define PER_BASE_POS				8U

static PCU_Type *get_pcu_from_port(peripheral_t port)
{
	PCU_Type *pcu = NULL;

	switch (port) {
	case PERI_GPIOA:
		pcu = PCA;
		break;
	case PERI_GPIOB:
		pcu = PCB;
		break;
	case PERI_GPIOC:
		pcu = PCC;
		break;
	case PERI_GPIOD:
		pcu = PCD;
		break;
	case PERI_GPIOE:
		pcu = PCE;
		break;
	case PERI_GPIOF:
		pcu = PCF;
		break;
	default:
		break;
	}

	assert(pcu != NULL);
	return pcu;
}

static GPIO_Type *get_reg_from_port(peripheral_t port)
{
	GPIO_Type *reg = NULL;

	switch (port) {
	case PERI_GPIOA:
		reg = PA;
		break;
	case PERI_GPIOB:
		reg = PB;
		break;
	case PERI_GPIOC:
		reg = PC;
		break;
	case PERI_GPIOD:
		reg = PD;
		break;
	case PERI_GPIOE:
		reg = PE;
		break;
	case PERI_GPIOF:
		reg = PF;
		break;
	default:
		break;
	}

	assert(reg != NULL);
	return reg;
}

static void enable_port(peripheral_t port)
{
	uint32_t pos = port - PERI_GPIOA;
	bitop_set(&PMU->PER, pos + PER_BASE_POS);
}

static void disable_port(peripheral_t port)
{
	uint32_t pos = port - PERI_GPIOA;
	bitop_clear(&PMU->PER, pos + PER_BASE_POS);
}

static void set_gpio_mode(PCU_Type *ctrl, uint32_t pin, gpio_mode_t mode)
{
	uint32_t pos = pin * 2;
	uint32_t val = 0;

	switch (mode) {
	case GPIO_MODE_OPENDRAIN: /* fall through */
	case GPIO_MODE_OPENDRAIN_PULLUP: /* fall through */
	case GPIO_MODE_OPENDRAIN_PULLDOWN:
		val = 1;
		break;
	case GPIO_MODE_INPUT: /* fall through */
	case GPIO_MODE_INPUT_PULLUP: /* fall through */
	case GPIO_MODE_INPUT_PULLDOWN:
		val = 2;
		break;
	case GPIO_MODE_ANALOG:
		val = 3;
		break;
	case GPIO_MODE_PUSHPULL:
	default:
		break;
	}

	bitop_clean_set_with_mask(&ctrl->CR, pos, 3U, val);
}

static void set_gpio_pullmode(PCU_Type *ctrl, uint32_t pin, gpio_mode_t mode)
{
	uint32_t reg = ctrl->PCR;

	bitop_clear(&reg, pin);
	bitop_clear(&reg, pin + 16);

	if (mode == GPIO_MODE_INPUT_PULLDOWN ||
			mode == GPIO_MODE_OPENDRAIN_PULLDOWN) {
		bitop_set(&reg, pin + 16);
		bitop_set(&reg, pin);
	} else if (mode == GPIO_MODE_INPUT_PULLUP ||
			mode == GPIO_MODE_OPENDRAIN_PULLUP) {
		bitop_set(&reg, pin);
	}

	ctrl->PCR = reg;
}

static void set_gpio_alt(PCU_Type *ctrl, uint32_t pin, int altfunc)
{
	uint32_t pos = pin * 2;
	bitop_clean_set_with_mask(&ctrl->MR, pos, 3U, (uint32_t)altfunc);
}

static void disable_gpio_irq(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->IER &= ~(3U << (pin * 2));
}

static void disable_gpio_debounce(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->DER &= ~(3U << pin);
}

static void set_gpio(peripheral_t port, uint32_t pin, gpio_mode_t mode)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);

	enable_port(port);
	set_gpio_mode(ctrl, pin, mode);
	set_gpio_pullmode(ctrl, pin, mode);
	set_gpio_alt(ctrl, pin, 0);
	disable_gpio_irq(ctrl, pin);
	disable_gpio_debounce(ctrl, pin);
}

void gpio_open(peripheral_t port, uint32_t pin, gpio_mode_t mode)
{
	set_gpio(port, pin, mode);
}

void gpio_close(peripheral_t port, uint32_t pin)
{
	set_gpio(port, pin, GPIO_MODE_ANALOG);
}

void gpio_set_altfunc(peripheral_t port, uint32_t pin, int altfunc)
{
	assert(altfunc < 4);
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	set_gpio_alt(ctrl, pin, altfunc);
}

void gpio_enable_irq(peripheral_t port, uint32_t pin, gpio_irq_t irq_type)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	uint32_t pos = pin * 2;
	uint32_t val = 0;
	bool edge = false;

	switch (irq_type) {
	case GPIO_IRQ_EDGE_RISING:
		edge = true;
		/* fall through */
	case GPIO_IRQ_LEVEL_HIGH:
		val = 2;
		break;
	case GPIO_IRQ_EDGE_FALLING:
		edge = true;
		/* fall through */
	case GPIO_IRQ_LEVEL_LOW:
		val = 1;
		break;
	case GPIO_IRQ_EDGE_ANY:
		edge = true;
		val = 3;
		break;
	default:
		break;
	}

	bitop_clean_set_with_mask(&ctrl->ICR, pos, 3U, val);
	bitop_clean_set_with_mask(&ctrl->IER, pos, 3U, ((uint32_t)edge << 1) + 1);
}

void gpio_disable_irq(peripheral_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	ctrl->IER &= ~(3U << (pin * 2));
}

void gpio_clear_event(peripheral_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	ctrl->ISR |= 1U << (pin * 2);
}

void gpio_write(peripheral_t port, uint32_t pin, int value)
{
	assert(pin < MAX_PIN_NUMBER);
	GPIO_Type *reg = get_reg_from_port(port);
	reg->SRR = 1U << (pin + ((uint32_t)!(value & 1) * 16));
}

int gpio_read(peripheral_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	const GPIO_Type *reg = get_reg_from_port(port);
	return (reg->IDR >> pin) & 0x1;
}

void gpio_write_port(peripheral_t port, int value)
{
	get_reg_from_port(port)->ODR = (uint32_t)value;
}

int gpio_read_port(peripheral_t port)
{
	const GPIO_Type *reg = get_reg_from_port(port);
	return (int)reg->IDR;
}

void gpio_enable_port(peripheral_t port)
{
	enable_port(port);
}

void gpio_disable_port(peripheral_t port)
{
	disable_port(port);
}

void gpio_reset(peripheral_t port)
{
	unused(port);
}
