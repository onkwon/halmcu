#include "halmcu/hal/wdt.h"
#include "halmcu/ll/pwr.h"
#include "halmcu/ll/clk.h"

static uint32_t get_clock_freqeuncy(void)
{
	return clk_ll_get_frequency(wdt_ll_get_clock_source())
		/ wdt_ll_get_prescaler();
}

uint32_t wdt_get_clock_frequency(void)
{
	return get_clock_freqeuncy();
}

void wdt_enable(void)
{
	pwr_ll_enable_peripheral(PERIPH_WDT);
	clk_ll_enable_peripheral(PERIPH_WDT);
}

void wdt_disable(void)
{
	clk_ll_disable_peripheral(PERIPH_WDT);
	pwr_ll_disable_peripheral(PERIPH_WDT);
}

void wdt_set_prescaler(uint32_t div_factor)
{
	wdt_ll_set_prescaler(div_factor);
}

uint32_t wdt_get_prescaler(void)
{
	return wdt_ll_get_prescaler();
}

void wdt_set_reload(uint32_t timeout)
{
	wdt_ll_set_reload(timeout);
}

uint32_t wdt_get_reload(void)
{
	return wdt_ll_get_reload();
}

void wdt_feed(void)
{
	wdt_ll_feed();
}

void wdt_start(void)
{
	wdt_ll_start();
}

void wdt_stop(void)
{
	wdt_ll_stop();
}

void wdt_set_debug_stop_mode(bool enable)
{
	wdt_ll_set_debug_stop_mode(enable);
}
