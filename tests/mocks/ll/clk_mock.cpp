#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/clk.h"

void clk_ll_enable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void clk_ll_disable_peripheral(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

uint32_t clk_ll_get_pclk_frequency(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(16000000);
}

uint32_t clk_ll_get_hclk_frequency(void)
{
	return mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(16000000);
}

uint32_t clk_ll_get_frequency(clk_source_t clk)
{
	return mock().actualCall(__func__)
		.withParameter("clk", clk)
		.returnUnsignedIntValueOrDefault(1000000);
}

clk_source_t clk_ll_get_peripheral_clock_source(periph_t peri)
{
	return (clk_source_t)mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(CLK_PLL);
}

bool clk_ll_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	return mock().actualCall(__func__)
		.withParameter("clk", clk)
		.withParameter("clkin", clkin)
		.withParameter("hz", hz)
		.returnBoolValueOrDefault(true);
}

void clk_ll_enable_source(clk_source_t clk)
{
	mock().actualCall(__func__).withParameter("clk", clk);
}

void clk_ll_start_pll(void)
{
	mock().actualCall(__func__);
}

bool clk_ll_is_pll_locked(void)
{
	return mock().actualCall(__func__).returnBoolValueOrDefault(true);
}

void clk_ll_set_source(clk_source_t clk)
{
	mock().actualCall(__func__).withParameter("clk", clk);
}

uint32_t clk_ll_get_peripheral_clock_source_frequency(periph_t peri)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(8000000);
}

void clk_ll_enable_output(void) {
	mock().actualCall(__func__);
}
void clk_ll_disable_output(void) {
	mock().actualCall(__func__);
}
void clk_ll_set_output_prescaler(uint32_t div_factor) {
	mock().actualCall(__func__).withParameter("div_factor", div_factor);
}
void clk_ll_set_output_source(clk_source_t clk) {
	mock().actualCall(__func__).withParameter("clk", clk);
}
void clk_ll_disable_source(clk_source_t clk) {
	mock().actualCall(__func__).withParameter("clk", clk);
}
clk_source_t clk_ll_get_source(void) {
	return (clk_source_t)mock().actualCall(__func__)
		.returnUnsignedIntValueOrDefault(CLK_LSI);
}
void clk_ll_set_peripheral_clock_source(periph_t peri, clk_source_t clk) {
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("clk", clk);
}
void clk_ll_stop_pll(void) {
	mock().actualCall(__func__);
}
