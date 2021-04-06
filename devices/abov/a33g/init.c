#include "abov/system.h"
#include "abov/errata.h"
#include "abov/ll/clk.h"
#include "abov/ll/wdt.h"

void pre_main(void)
{
	errata_1();
	wdt_stop();

	clk_enable_source(ABOV_CLK_HSI);
	clk_set_source(ABOV_CLK_PLL_BYPASS);

	interrupt_unlock();
}
