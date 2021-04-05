#include "abov/system.h"
#include "abov/errata.h"
#include "abov/hal/clk.h"
#include "abov/hal/wdt.h"

void pre_main(void)
{
	errata_1();
	wdt_stop();

	clk_enable_source(CLK_INTERNAL_OSC_16MHZ);
	clk_set_source(CLK_PLL_BYPASS);

	interrupt_unlock();
}
