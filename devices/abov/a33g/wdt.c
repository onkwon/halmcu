#include "abov/ll/wdt.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define WPRS_POS				(0)
#define WEN_POS					(5)
#define WRE_POS					(6)
#define WIE_POS					(7)
#define WOF_POS					(8)
#define WDH_POS					(15)

#define WPRS_MASK				(7UL << WPRS_POS)

#define WEN					(1UL << WEN_POS)
#define WRE					(1UL << WRE_POS)
#define WIE					(1UL << WIE_POS)
#define WDH					(1UL << WDH_POS)

void wdt_ll_set_prescaler(uint32_t div_factor)
{
	uint32_t val = WDT->CON & ~WPRS_MASK;
	val |= (div_factor << WPRS_POS) & WPRS_MASK;
	WDT->CON = val;
}

void wdt_ll_reload(uint32_t timeout)
{
	WDT->LR = timeout;
}

void wdt_ll_start(void)
{
	WDT->CON |= WEN;
}

void wdt_ll_stop(void)
{
	WDT->CON &= ~WEN;
}

void wdt_ll_set_debug_hold_mode(bool enable)
{
	uint32_t val = WDT->CON & ~WDH;
	val |= (uint32_t)enable << WDH_POS;
	WDT->CON = val;
}

bool wdt_ll_is_event_raised(void)
{
	return !!((WDT->CON >> WOF_POS) & 1);
}

void wdt_ll_set_interrupt(bool enable)
{
	uint32_t val = WDT->CON & ~(WIE | WRE);
	val |= ((uint32_t)enable << WIE_POS) | ((uint32_t)!enable << WRE_POS);
	WDT->CON = val;
}

uint32_t wdt_ll_get_count(void)
{
	return WDT->CVR;
}

uint32_t wdt_ll_get_timeout(void)
{
	return WDT->LR;
}

#if 0
void wdt_hal_enable(void)
{
}

void wdt_hal_disable(void)
{
}
#endif
