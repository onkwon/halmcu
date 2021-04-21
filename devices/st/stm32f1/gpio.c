#include "abov/ll/gpio.h"

#include <stddef.h>
#include <assert.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "stm32f1.h"
#include "exti.h"

#define MAX_PIN_NUMBER				16

static GPIO_Type *get_reg_from_port(peripheral_t port)
{
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

	assert(0);
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

void gpio_enable_irq(peripheral_t port, uint32_t pin, gpio_irq_t irq_type)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	assert(pin < MAX_PIN_NUMBER);

	exti_t exti = (exti_t)pin;
	exti_set_source(port, pin);

	exti_clear_rising_trigger(exti);
	exti_clear_falling_trigger(exti);

	if (irq_type == GPIO_IRQ_EDGE_RISING) {
		exti_set_rising_trigger(exti);
	} else if (irq_type == GPIO_IRQ_EDGE_FALLING) {
		exti_set_falling_trigger(exti);
	} else if (irq_type == GPIO_IRQ_EDGE_ANY) {
		exti_set_rising_trigger(exti);
		exti_set_falling_trigger(exti);
	}

	exti_enable_irq(exti);
}

void gpio_disable_irq(peripheral_t port, uint32_t pin)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	assert(pin < MAX_PIN_NUMBER);

	exti_t exti = (exti_t)pin;
	exti_disable_irq(exti);
}

void gpio_clear_event(peripheral_t port, uint32_t pin)
{
	assert(port >= PERI_GPIOA && port <= PERI_GPIOG);
	assert(pin < MAX_PIN_NUMBER);

	exti_t exti = (exti_t)pin;
	exti_clear_event(exti);
}

void gpio_set_mode(peripheral_t port, uint32_t pin, gpio_mode_t mode)
{
	assert(pin < MAX_PIN_NUMBER);

	uint32_t val = 0;

	switch (mode) {
	case GPIO_MODE_INPUT: /* floating input */
		val = 0x4;
		break;
	case GPIO_MODE_INPUT_PULLUP: /* fall through */
	case GPIO_MODE_INPUT_PULLDOWN:
		val = 0x8;
		break;
	case GPIO_MODE_PUSHPULL:
		val = 0x2;
		break;
	case GPIO_MODE_OPENDRAIN: /* fall through */
	case GPIO_MODE_OPENDRAIN_PULLUP: /* fall through */
	case GPIO_MODE_OPENDRAIN_PULLDOWN:
		val = 0x6;
		break;
	default:
		break;
	}

	uint32_t pos = (pin % 8) * 4;
	volatile uint32_t *reg = ((pin / 8) == 0)?
		&get_reg_from_port(port)->CRL : &get_reg_from_port(port)->CRH;

	bitop_clean_set_with_mask(reg, pos, 0xf, val);
}

void gpio_set_altfunc(peripheral_t port, uint32_t pin, int altfunc)
{
	assert(pin < MAX_PIN_NUMBER);
	unused(altfunc);

	volatile uint32_t *reg = ((pin / 8) == 0)?
		&get_reg_from_port(port)->CRL : &get_reg_from_port(port)->CRH;
	uint32_t pos = (pin % 8) * 4;
	uint32_t val = (*reg >> pos) & 0xfU;
#if 0
	if ((val & 0x3) == 0) { /* in case of input mode */
		return;
	}
#endif
	val |= 0x8;

	bitop_clean_set_with_mask(reg, pos, 0xf, val);
}

void gpio_set_speed(peripheral_t port, uint32_t pin, gpio_speed_t speed)
{
	assert(pin < MAX_PIN_NUMBER);

	volatile uint32_t *reg = ((pin / 8) == 0)?
		&get_reg_from_port(port)->CRL : &get_reg_from_port(port)->CRH;
	uint32_t pos = (pin % 8) * 4;
	uint32_t val = (*reg >> pos) & 0xcU;
#if 0
	if ((val & 0x3) == 0) { /* in case of input mode */
		return;
	}
#endif
	if (speed == GPIO_SPEED_HIGH) {
		val |= 3;
	} else if (speed == GPIO_SPEED_MID) {
		val |= 1;
	} else {
		val |= 2;
	}

	bitop_clean_set_with_mask(reg, pos, 0xf, val);
}

void gpio_set_debouncer(peripheral_t port, uint32_t pin, uint32_t pclk_clocks)
{
	unused(port);
	unused(pin);
	unused(pclk_clocks);
}
