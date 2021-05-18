#include "abov/hal/clk.h"

bool clk_init(clk_source_t clock_source, uint32_t target_hz)
{
	clk_ll_enable_source(clock_source);
	if (!clk_ll_set_pll_frequency(CLK_PLL, clock_source, target_hz)) {
		return false;
	}

	clk_ll_start_pll();
	while (!clk_ll_is_pll_locked()) ;

	clk_ll_set_source(CLK_PLL);

	return true;
}

void clk_enable_peripheral(periph_t peri)
{
	clk_ll_enable_peripheral(peri);
}

void clk_disable_peripheral(periph_t peri)
{
	clk_ll_disable_peripheral(peri);
}

void clk_enable_source(clk_source_t clk)
{
	clk_ll_enable_source(clk);
}

void clk_disable_source(clk_source_t clk)
{
	clk_ll_disable_source(clk);
}

void clk_set_source(clk_source_t clk)
{
	clk_ll_set_source(clk);
}

clk_source_t clk_get_source(void)
{
	return clk_ll_get_source();
}

bool clk_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	return clk_ll_set_pll_frequency(clk, clkin, hz);
}

void clk_start_pll(void)
{
	clk_ll_start_pll();
}

void clk_stop_pll(void)
{
	clk_ll_stop_pll();
}

bool clk_is_pll_locked(void)
{
	return clk_ll_is_pll_locked();
}

uint32_t clk_get_hclk_frequency(void)
{
	return clk_ll_get_hclk_frequency();
}

uint32_t clk_get_pclk_frequency(void)
{
	return clk_ll_get_pclk_frequency();
}

uint32_t clk_get_frequency(clk_source_t clk)
{
	return clk_ll_get_frequency(clk);
}

clk_source_t clk_get_peripheral_clock_source(periph_t peri)
{
	return clk_ll_get_peripheral_clock_source(peri);
}

void clk_set_peripheral_clock_source(periph_t peri, clk_source_t clk)
{
	clk_ll_set_peripheral_clock_source(peri, clk);
}

uint32_t clk_get_peripheral_clock_source_frequency(periph_t peri)
{
	return clk_ll_get_peripheral_clock_source_frequency(peri);
}

void clk_enable_output(void)
{
	clk_ll_enable_output();
}

void clk_disable_output(void)
{
	clk_ll_disable_output();
}

void clk_set_output_prescaler(uint32_t div_factor)
{
	clk_ll_set_output_prescaler(div_factor);
}

void clk_set_output_source(clk_source_t clk)
{
	clk_ll_set_output_source(clk);
}
