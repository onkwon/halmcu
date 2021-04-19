#include "abov/ll/gpio.h"

#include <stddef.h>
#include <assert.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "stm32f1.h"

#define MAX_PIN_NUMBER				16

static GPIO_Type *get_reg_from_port(peripheral_t port)
{
	GPIO_Type *reg = NULL;

	switch (port) {
	case PERI_GPIOA:
		return GPIOA;
	case PERI_GPIOB:
		return GPIOB;
	case PERI_GPIOC:
		return GPIOC;
	case PERI_GPIOD:
		return GPIOD;
	case PERI_GPIOE:
		return GPIOE;
	case PERI_GPIOF:
		return GPIOF;
	case PERI_GPIOG:
		return GPIOG;
	default:
		break;
	}

	assert(reg != NULL);
	return GPIOA;
}

static void enable_port(peripheral_t port)
{
	uint32_t pos = port - PERI_GPIOA + 2;
	bitop_set(&RCC->APB2ENR, pos);
}

static void disable_port(peripheral_t port)
{
	uint32_t pos = port - PERI_GPIOA + 2;
	bitop_clear(&RCC->APB2ENR, pos);
}

void gpio_reset(peripheral_t port)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	uint32_t pos = port - PERI_GPIOA + 2;
	bitop_set(&RCC->APB2RSTR, pos);
}

void gpio_enable_port(peripheral_t port)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	enable_port(port);
}

void gpio_disable_port(peripheral_t port)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	disable_port(port);
}

int gpio_read_port(peripheral_t port)
{
	return (int)get_reg_from_port(port)->IDR;
}

void gpio_write_port(peripheral_t port, int value)
{
	get_reg_from_port(port)->ODR = (uint32_t)value;
}

int gpio_read(peripheral_t port, uint32_t pin)
{
	assert(pin < MAX_PIN_NUMBER);
	return (get_reg_from_port(port)->IDR >> pin) & 0x1;
}

void gpio_write(peripheral_t port, uint32_t pin, int value)
{
	assert(pin < MAX_PIN_NUMBER);
	uint32_t pos = !value * 16U + pin;
	get_reg_from_port(port)->BSRR = 1U << pos;
}
