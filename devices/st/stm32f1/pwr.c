#include "halmcu/ll/pwr.h"
#include "halmcu/compiler.h"

HALMCU_WEAK
void pwr_ll_enable_peripheral(periph_t peri)
{
	unused(peri);
}

HALMCU_WEAK
void pwr_ll_disable_peripheral(periph_t peri)
{
	unused(peri);
}

HALMCU_WEAK
void pwr_ll_reset(void)
{
	/* place holder */
}
