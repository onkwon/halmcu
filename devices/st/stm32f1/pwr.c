#include "abov/ll/pwr.h"
#include "abov/compiler.h"

ABOV_WEAK
void pwr_ll_enable_peripheral(periph_t peri)
{
	unused(peri);
}

ABOV_WEAK
void pwr_ll_disable_peripheral(periph_t peri)
{
	unused(peri);
}

ABOV_WEAK
void pwr_ll_reset(void)
{
	/* place holder */
}
