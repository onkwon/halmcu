#include "abov/asm/arm/systick.h"
#include "abov/asm/arm/cmsis.h"
#include "abov/ll/clk.h"

#if !defined(SYSTICK_PRESCALER)
#define SYSTICK_PRESCALER			8
#endif
#define SYSTICK_RESOLUTION			SysTick_LOAD_RELOAD_Msk

static uint32_t get_reload(void)
{
	return SysTick->LOAD;
}

static void set_reload(uint32_t period)
{
	SysTick->LOAD = period;
}

static uint32_t get_prescaler(void)
{
	if ((SysTick->CTRL >> SysTick_CTRL_CLKSOURCE_Pos) == 1/*hclk*/) {
		return 1;
	}

	/* vendor specific */
	return SYSTICK_PRESCALER;
}

void systick_clear(void)
{
	SysTick->VAL = 0;
}

uint32_t systick_get_counter(void)
{
	return SysTick->VAL;
}

void systick_set_counter(uint32_t value)
{
	SysTick->VAL = value;
}

void systick_start(void)
{
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

void systick_stop(void)
{
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk);
}

uint32_t systick_set_frequency(uint32_t hz)
{
	uint32_t div = get_prescaler();
	uint32_t clk = clk_ll_get_hclk_frequency() / div;
	uint32_t period = clk / hz;

	if (period <= 0 || period > SYSTICK_RESOLUTION) {
		return 0;
	}

	set_reload(period - 1);

	return clk / period;
}

uint32_t systick_get_frequency(void)
{
	return clk_ll_get_hclk_frequency() / (get_reload() + 1);
}
