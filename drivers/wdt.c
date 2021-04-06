#include "abov/drivers/wdt.h"
#include "abov/hal/pwr.h"
#include "abov/hal/clk.h"

#define KHZ			1000

static uint32_t get_clock_freqeuncy(void)
{
	return clk_get_frequency(wdt_get_clock_source()) / wdt_get_prescaler();
}

void wdt_enable(void)
{
	pwr_enable_peripheral(PERIPHERAL_WDT);
	clk_enable_peripheral(PERIPHERAL_WDT);
}

void wdt_disable(void)
{
	pwr_disable_peripheral(PERIPHERAL_WDT);
	clk_disable_peripheral(PERIPHERAL_WDT);
}

uint32_t wdt_get_clock_frequency(void)
{
	return get_clock_freqeuncy();
}

void wdt_set_ms(uint32_t period_ms)
{
	uint32_t khz = get_clock_freqeuncy() / KHZ;
	uint32_t ticks = khz * period_ms;
	wdt_reload(ticks);
}

void wdt_feed(void)
{
	wdt_reload(wdt_get_reload());
}
