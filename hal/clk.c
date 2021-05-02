#include "abov/hal/clk.h"

bool clk_init(clk_source_t clock_source, uint32_t target_hz)
{
	clk_enable_source(clock_source);
	if (!clk_set_pll_frequency(CLK_PLL, clock_source, target_hz)) {
		return false;
	}

	clk_start_pll();
	while (!clk_is_pll_locked()) ;

	clk_set_source(CLK_PLL);

	return true;
}
