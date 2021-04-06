#include "abov/ll/wdt.h"
#include "abov/bitop.h"

#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define WPRS_POS				(0)
#define WEN_POS					(5)
#define WRE_POS					(6)
#define WIE_POS					(7)
#define WOF_POS					(8)
#define WDH_POS					(15)

#define WPRS_MASK				(7U << WPRS_POS)

#define WEN					(1U << WEN_POS)
#define WRE					(1U << WRE_POS)
#define WIE					(1U << WIE_POS)
#define WDH					(1U << WDH_POS)

void wdt_set_prescaler(uint32_t div_factor)
{
	uint32_t val = WDT->CON & ~WPRS_MASK;
	val |= (div_factor << WPRS_POS) & WPRS_MASK;
	WDT->CON = val;
}

uint32_t wdt_get_prescaler(void)
{
	uint32_t val = (WDT->CON & WPRS_MASK) >> WPRS_POS;
	if (val == 0) {
		return 1;
	}
	return 1U << (val + 1);
}

void wdt_reload(uint32_t timeout)
{
	if (WDT->CON & WIE && timeout > 0) {
		timeout -= 1;
	}

	WDT->LR = timeout;
}

uint32_t wdt_get_reload(void)
{
	return WDT->LR;
}

void wdt_start(void)
{
	WDT->CON |= WEN;
}

void wdt_stop(void)
{
	WDT->CON &= ~WEN;
}

void wdt_set_debug_hold_mode(bool enable)
{
	uint32_t val = WDT->CON & ~WDH;
	val |= (uint32_t)enable << WDH_POS;
	WDT->CON = val;
}

bool wdt_is_event_raised(void)
{
	return !!((WDT->CON >> WOF_POS) & 1);
}

void wdt_set_interrupt(bool enable)
{
	uint32_t val = WDT->CON & ~(WIE | WRE);
	val |= ((uint32_t)enable << WIE_POS) | ((uint32_t)!enable << WRE_POS);
	WDT->CON = val;

	PMU->RSER |= 1U << 3;
}

uint32_t wdt_get_count(void)
{
	return WDT->CVR;
}

#include "abov/hal/wdt.h"
#include "abov/ll/clk.h"

void wdt_set_clock_source(clk_source_t clk)
{
	uint32_t val = 0;

	switch (clk) {
	case CLK_INTERNAL_OSC:
		val = 3;
		break;
	case CLK_INTERNAL_OSC_16MHZ:
		val = 1;
		break;
	case CLK_PLL:
		val = 0;
		break;
	case CLK_EXTERNAL_OSC_SUB:
		val = 2;
		break;
	default:
		return;
	}

	bitop_clean_set_with_mask(&PMU->PCSR, 0, 3U, val);
	WDT->CON |= 1U << 3;
}

clk_source_t wdt_get_clock_source(void)
{
	switch (PMU->PCSR & 3) {
	case 1:
		return CLK_INTERNAL_OSC_16MHZ;
	case 2:
		return CLK_EXTERNAL_OSC_SUB;
	case 3:
		return CLK_INTERNAL_OSC;
	case 0:
	default:
		return CLK_PLL;
	}
}
