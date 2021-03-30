#include "abov/ll/pwr.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#include <assert.h>

#define PMUMODE_POS				0
#define VDCLP_POS				2

#define PMUMODE_MASK				(3U << PMUMODE_POS)
#define VDCLP_MASK				(1U << VDCLP_POS)

#define SWRST					1

static uint32_t get_wakeup_bitmask_from_peripheral(uint32_t x)
{
	switch (x) {
	case LL_PERIPHERAL_GPIO + 0: // GPIOA
		return 1U << 5;
	case LL_PERIPHERAL_GPIO + 1: // GPIOB
		return 1U << 6;
	case LL_PERIPHERAL_GPIO + 2: // GPIOC
		return 1U << 7;
	case LL_PERIPHERAL_GPIO + 3: // GPIOD
		return 1U << 8;
	case LL_PERIPHERAL_GPIO + 4: // GPIOE
		return 1U << 9;
	case LL_PERIPHERAL_GPIO + 5: // GPIOF
		return 1U << 10;
	case LL_PERIPHERAL_FRT:
		return 1U << 4;
	case LL_PERIPHERAL_WDT:
		return 1U << 3;
	case LL_PERIPHERAL_MXFAIL:
		return 1U << 1;
	case LL_PERIPHERAL_LVD:
		return 1U << 0;
	}

	return 0;
}

void pwr_ll_reboot(void)
{
	PMU->CFG |= SWRST;
}

uint32_t pwr_ll_get_reboot_source(void)
{
	return PMU->RSSR;
}

void pwr_ll_clear_reboot_source(uint32_t bitpos)
{
	PMU->RSSR &= ~bitpos;
}

void pwr_ll_set_mode(uint32_t val)
{
	uint32_t bit = 0;
	PMU->MR &= ~(PMUMODE_MASK | VDCLP_MASK);

	switch (val) {
	case LL_POWER_MODE_RUN:
		bit = 0;
		break;
	case LL_POWER_MODE_SLEEP:
		bit = (1U << PMUMODE_POS);
		break;
	case LL_POWER_MODE_DEEP_SLEEP:
		bit = (2U << PMUMODE_POS);
		break;
	case LL_POWER_MODE_BLACKOUT:
		bit = (2U << PMUMODE_POS) | (1U << VDCLP_POS);
		break;
	default:
		break;
	}

	PMU->MR |= bit;
}

void pwr_ll_set_wakeup_source(uint32_t val)
{
	PMU->WSER |= get_wakeup_bitmask_from_peripheral(val);
}

void pwr_ll_clear_wakeup_source(uint32_t val)
{
	PMU->WSER &= ~get_wakeup_bitmask_from_peripheral(val);
}

uint32_t pwr_ll_get_wakeup_source(void)
{
	return PMU->WSSR;
}
