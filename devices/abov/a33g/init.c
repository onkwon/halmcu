#include "halmcu/system.h"
#include "halmcu/periph/clk.h"
#include "halmcu/periph/wdt.h"
#include "errata.h"

void pre_main(void)
{
	errata_1();
	wdt_stop();

	clk_enable_source(CLK_HSI);
	clk_set_source(CLK_PLL_BYPASS);

	interrupt_unlock();
}
