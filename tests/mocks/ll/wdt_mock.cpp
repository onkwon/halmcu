#include "CppUTestExt/MockSupport.h"

#include "abov/ll/wdt.h"

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
