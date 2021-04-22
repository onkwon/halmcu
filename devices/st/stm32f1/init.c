#include "abov/system.h"
#include "abov/ll/clk.h"

void pre_main(void)
{
	clk_enable_peripheral(PERIPH_AFIO);
}
