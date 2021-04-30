#include "abov/ll/pwr.h"
#include "abov/compiler.h"

ABOV_WEAK
void pwr_enable_peripheral(periph_t peri)
{
	unused(peri);
}

ABOV_WEAK
void pwr_disable_peripheral(periph_t peri)
{
	unused(peri);
}

ABOV_WEAK
void pwr_reset(void)
{
	/* place holder */
}
