#include "halmcu/periph/pwr.h"
#include "halmcu/compiler.h"

HALMCU_WEAK
void pwr_enable_peripheral(periph_t peri)
{
	unused(peri);
}

HALMCU_WEAK
void pwr_disable_peripheral(periph_t peri)
{
	unused(peri);
}

HALMCU_WEAK
void pwr_reset(void)
{
	/* place holder */
}
