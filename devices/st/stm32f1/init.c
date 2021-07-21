#include "halmcu/system.h"
#include "halmcu/ll/clk.h"

void pre_main(void)
{
	clk_enable_peripheral(PERIPH_AFIO);
}
