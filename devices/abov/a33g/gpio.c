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

static uint32_t get_log2(uint32_t value)
{
	uint32_t cnt = 0;
	for (cnt = 0; value != 0; value >>= 1) {
		cnt++;
	}
	return cnt;
}

static PCU_Type *get_pcu_from_port(periph_t port)
{
	switch (port) {
	case PERIPH_GPIOA:
		return PCA;
	case PERIPH_GPIOB:
		return PCB;
	case PERIPH_GPIOC:
		return PCC;
	case PERIPH_GPIOD:
		return PCD;
	case PERIPH_GPIOE:
		return PCE;
	case PERIPH_GPIOF:
		return PCF;
	default:
		assert(0);
		return PCA;
	}
}

static GPIO_Type *get_reg_from_port(periph_t port)
{
	switch (port) {
	case PERIPH_GPIOA:
		return PA;
	case PERIPH_GPIOB:
		return PB;
	case PERIPH_GPIOC:
		return PC;
	case PERIPH_GPIOD:
		return PD;
	case PERIPH_GPIOE:
		return PE;
	case PERIPH_GPIOF:
		return PF;
	default:
		assert(0);
		return PA;
	}
}

static void enable_port(periph_t port)
{
	uint32_t pos = port - PERIPH_GPIOA;
	bitop_set(&PMU->PER, pos + PER_BASE_POS);
}

static void disable_port(periph_t port)
{
	uint32_t pos = port - PERIPH_GPIOA;
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

void gpio_set_mode(periph_t port, uint32_t pin, gpio_mode_t mode)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	set_gpio_mode(ctrl, pin, mode);
	set_gpio_pullmode(ctrl, pin, mode);
}

void gpio_set_altfunc(periph_t port, uint32_t pin, int altfunc)
{
	assert(altfunc < 4);
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	set_gpio_alt(ctrl, pin, altfunc);
}

void gpio_enable_irq(periph_t port, uint32_t pin, gpio_irq_t irq_type)
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

void gpio_disable_irq(periph_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	ctrl->IER &= ~(3U << (pin * 2));
}

void gpio_clear_event(periph_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	PCU_Type *ctrl = get_pcu_from_port(port);
	ctrl->ISR |= 1U << (pin * 2);
}

void gpio_write(periph_t port, uint32_t pin, int value)
{
	assert(pin < MAX_PIN_NUMBER);
	GPIO_Type *reg = get_reg_from_port(port);
	reg->SRR = 1U << (pin + ((uint32_t)!(value & 1) * 16));
}

int gpio_read(periph_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	const GPIO_Type *reg = get_reg_from_port(port);
	return (reg->IDR >> pin) & 0x1;
}

void gpio_write_port(periph_t port, int value)
{
	get_reg_from_port(port)->ODR = (uint32_t)value;
}

int gpio_read_port(periph_t port)
{
	const GPIO_Type *reg = get_reg_from_port(port);
	return (int)reg->IDR;
}

void gpio_enable_port(periph_t port)
{
	assert(port >= PERIPH_GPIOA && port <= PERIPH_GPIOF);
	enable_port(port);
}

void gpio_disable_port(periph_t port)
{
	assert(port >= PERIPH_GPIOA && port <= PERIPH_GPIOF);
	disable_port(port);
}

void gpio_reset(periph_t port)
{
	unused(port);
}

void gpio_set_debouncer(periph_t port, uint32_t pin, uint32_t pclk_clocks)
{
	assert(pin < MAX_PIN_NUMBER);

	PCU_Type *ctrl = get_pcu_from_port(port);

	if (pclk_clocks == 0) {
		bitop_clear(&ctrl->DER, pin);
		return;
	}

	ctrl->DPR = get_log2(pclk_clocks) - 3;
	bitop_set(&ctrl->DER, pin);
}

void gpio_set_speed(periph_t port, uint32_t pin, gpio_speed_t speed)
{
	unused(port);
	unused(pin);
	unused(speed);
}
