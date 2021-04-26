#include "abov/hal/wdt.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

#define KHZ			1000

static uint32_t get_clock_freqeuncy(void)
{
	return clk_get_frequency(wdt_get_clock_source()) / wdt_get_prescaler();
}

uint32_t wdt_get_clock_frequency(void)
{
	return get_clock_freqeuncy();
}

void wdt_enable(void)
{
	pwr_enable_peripheral(PERIPH_WDT);
	clk_enable_peripheral(PERIPH_WDT);
}

void wdt_disable(void)
{
	pwr_disable_peripheral(PERIPH_WDT);
	clk_disable_peripheral(PERIPH_WDT);
}

void wdt_set_reload_ms(uint32_t period_ms)
{
	uint32_t khz = get_clock_freqeuncy() / KHZ;
	uint32_t ticks = khz * period_ms;
	wdt_set_reload(ticks);
}
