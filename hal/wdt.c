#include "abov/hal/wdt.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

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
	clk_disable_peripheral(PERIPH_WDT);
	pwr_disable_peripheral(PERIPH_WDT);
}
