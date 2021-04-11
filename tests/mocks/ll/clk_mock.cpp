#include "CppUTestExt/MockSupport.h"

#include "abov/ll/clk.h"

void clk_enable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void clk_disable_peripheral(peripheral_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

uint32_t clk_get_pclk_frequency(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(16000000);
}
