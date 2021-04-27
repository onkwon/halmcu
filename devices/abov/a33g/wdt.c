#include "abov/ll/wdt.h"
#include "abov/bitop.h"

#include <assert.h>

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

static void set_reload(uint32_t timeout)
{
	WDT->LR = timeout;
}

static uint32_t get_prescaler_value_from_divisor(uint32_t divisor)
{
	uint32_t i = 0;

	while (divisor != 0) {
		divisor >>= 1;
		i++;
	}

	if (i >= 3) {
		return i - 2;
	}

	return 0;
}

void wdt_set_prescaler(uint32_t div_factor)
{
	assert(is_pwr2(div_factor) && div_factor <= 256 && div_factor != 2);

	uint32_t val = get_prescaler_value_from_divisor(div_factor);
	uint32_t reg = WDT->CON & ~WPRS_MASK;
	reg |= (val << WPRS_POS) & WPRS_MASK;
	WDT->CON = reg;
}

uint32_t wdt_get_prescaler(void)
{
	uint32_t prescaler = (WDT->CON & WPRS_MASK) >> WPRS_POS;
	if (prescaler == 0) {
		return 1;
	}
	return 1U << (prescaler + 1);
}

void wdt_set_reload(uint32_t timeout)
{
	if ((WDT->CON & WIE) && timeout > 0) {
		timeout -= 1;
	}

	set_reload(timeout);
}

uint32_t wdt_get_reload(void)
{
	return WDT->LR;
}

void wdt_feed(void)
{
	set_reload(WDT->LR);
}

void wdt_start(void)
{
	uint32_t val = WDT->CON;
	if (!(val & (1U << WIE_POS))) {
		PMU->RSER |= 1U << 3;
		val |= (1U << WRE_POS);
	}
	val |= WEN;

	WDT->CON = val;
}

void wdt_stop(void)
{
	WDT->CON &= ~WEN;
}

void wdt_set_debug_stop_mode(bool enable)
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

void wdt_set_clock_source(clk_source_t clk)
{
	uint32_t val = 0;

	switch (clk) {
	case CLK_LSI:
		val = 3;
		break;
	case CLK_HSI:
		val = 1;
		break;
	case CLK_PLL:
		val = 0;
		break;
	case CLK_LSE:
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
		return CLK_HSI;
	case 2:
		return CLK_LSE;
	case 3:
		return CLK_LSI;
	case 0:
	default:
		return CLK_PLL;
	}
}
