#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/wdt.h"

uint32_t wdt_get_prescaler(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(1);
}

clk_source_t wdt_get_clock_source(void)
{
	return (clk_source_t)mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(CLK_PLL);
}

void wdt_set_reload(uint32_t timeout)
{
	mock().actualCall(__func__)
		.withParameter("timeout", timeout);
}

uint32_t wdt_get_reload(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(0);
}

void wdt_set_prescaler(uint32_t div_factor)
{
	mock().actualCall(__func__)
		.withParameter("div_factor", div_factor);
}

void wdt_feed(void)
{
	mock().actualCall(__func__);
}

void wdt_start(void)
{
	mock().actualCall(__func__);
}

void wdt_stop(void)
{
	mock().actualCall(__func__);
}

void wdt_set_debug_stop_mode(bool enable)
{
	mock().actualCall(__func__)
		.withParameter("enable", enable);
}
