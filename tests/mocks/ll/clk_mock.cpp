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

uint32_t clk_get_hclk_frequency(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(16000000);
}

uint32_t clk_get_frequency(clk_source_t clk)
{
	return mock().actualCall(__func__)
		.withParameter("clk", clk)
		.returnUnsignedIntValueOrDefault(1000000);
}

clk_source_t clk_get_peripheral_clock_source(peripheral_t peri)
{
	return (clk_source_t)mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(CLK_PLL);
}
