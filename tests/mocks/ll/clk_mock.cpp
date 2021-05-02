#include "CppUTestExt/MockSupport.h"

#include "abov/ll/clk.h"

void clk_enable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void clk_disable_peripheral(periph_t peri)
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

clk_source_t clk_get_peripheral_clock_source(periph_t peri)
{
	return (clk_source_t)mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(CLK_PLL);
}

bool clk_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	return mock().actualCall(__func__)
		.withParameter("clk", clk)
		.withParameter("clkin", clkin)
		.withParameter("hz", hz)
		.returnBoolValueOrDefault(true);
}

void clk_enable_source(clk_source_t clk)
{
	mock().actualCall(__func__).withParameter("clk", clk);
}

void clk_start_pll(void)
{
	mock().actualCall(__func__);
}

bool clk_is_pll_locked(void)
{
	return mock().actualCall(__func__).returnBoolValueOrDefault(true);
}

void clk_set_source(clk_source_t clk)
{
	mock().actualCall(__func__).withParameter("clk", clk);
}

uint32_t clk_get_peripheral_clock_source_frequency(periph_t peri)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(8000000);
}
