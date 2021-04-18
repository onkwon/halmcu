#include "exti.h"

#include <assert.h>

#include "stm32f1.h"
#include "abov/bitop.h"
#include "abov/ll/gpio.h"

void exti_enable_irq(exti_t exti)
{
	bitop_set(&EXTI->IMR, exti);
}

void exti_disable_irq(exti_t exti)
{
	bitop_clear(&EXTI->IMR, exti);
}

void exti_enable_event(exti_t exti)
{
	bitop_set(&EXTI->EMR, exti);
}

void exti_disable_event(exti_t exti)
{
	bitop_clear(&EXTI->EMR, exti);
}

void exti_set_source(uint32_t ngpio)
{
	volatile uint32_t *p[] = {
		&SYSCFG->CR1,
		&SYSCFG->CR2,
		&SYSCFG->CR3,
		&SYSCFG->CR4,
	};

	uint32_t pin = ngpio & (GPIO_PORT_SIZE - 1);
	gpio_port_t port = (gpio_port_t)(ngpio & ~(GPIO_PORT_SIZE - 1));
	uint32_t index = pin / 4;
	uint32_t bit = pin % 4 * 4;
	assert(index < (sizeof(p) / sizeof(*p)));
	assert(bit < 16);

	bitop_clean_set_with_mask(p[index], bit, 0xfU,
			port >> GPIO_PORT_SIZE_PWR2);
}

void exti_set_rising_trigger(exti_t exti)
{
	bitop_set(&EXTI->RTSR, exti);
}

void exti_clear_rising_trigger(exti_t exti)
{
	bitop_clear(&EXTI->RTSR, exti);
}

void exti_set_falling_trigger(exti_t exti)
{
	bitop_set(&EXTI->FTSR, exti);
}

void exti_clear_falling_trigger(exti_t exti)
{
	bitop_clear(&EXTI->FTSR, exti);
}

void exti_raise_irq(exti_t exti)
{
	bitop_set(&EXTI->SWIER, exti);
}

void exti_clear_event(exti_t exti)
{
	bitop_clear(&EXTI->PR, exti);
}

uint32_t exti_get_event(void)
{
	return EXTI->PR;
}
