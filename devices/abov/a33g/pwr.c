#include "abov/ll/pwr.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#include <assert.h>

#define PMUMODE_POS				0
#define VDCLP_POS				2

#define PMUMODE_MASK				(3U << PMUMODE_POS)
#define VDCLP_MASK				(1U << VDCLP_POS)

#define SWRST					1

static uint32_t get_wakeup_bitmask_from_enum(uint32_t x)
{
	uint32_t peri = x & ~(PERIPHERAL_SPACE_SIZE - 1);

	switch (peri) {
	case PERIPHERAL_GPIO:
		return 1U << (x - PERIPHERAL_GPIO + 5);
	case PERIPHERAL_FRT:
		return 1U << 4;
	case PERIPHERAL_WDT:
		return 1U << 3;
	case PERIPHERAL_MXFAIL:
		return 1U << 1;
	case PERIPHERAL_LVD:
		return 1U << 0;
	default:
		return 0;
	}
}

static uint32_t get_activation_bitmask_from_enum(uint32_t x)
{
	uint32_t peri = x & ~(PERIPHERAL_SPACE_SIZE - 1);

	switch (peri) {
	case PERIPHERAL_JTAG:
		return 1U << 31;
	case PERIPHERAL_PMC:
		return 1U << 29;
	case PERIPHERAL_ADC:
		return 1U << 28;
	case PERIPHERAL_PWM:
		return 1U << (((x - PERIPHERAL_PWM) >> 2) + 24);
	case PERIPHERAL_UART:
		return 1U << (x - PERIPHERAL_UART + 20);
	case PERIPHERAL_I2C:
		return 1U << (x - PERIPHERAL_I2C + 18);
	case PERIPHERAL_SPI:
		return 1U << (x - PERIPHERAL_SPI + 16);
	case PERIPHERAL_CRC:
		return 1U << 14;
	case PERIPHERAL_GPIO:
		return 1U << (x - PERIPHERAL_GPIO + 8);
	case PERIPHERAL_TIMER:
		if (x > PERIPHERAL_TIMER5) { /* TIMER6 ~ TIMER9 */
			return 1U << 7;
		} else if (x >= PERIPHERAL_TIMER2) { /* TIMER2 ~ TIMER5 */
			return 1U << 6;
		}
		return 1U << 5; /* TIMER0 and TIMER1 */
	case PERIPHERAL_FRT:
		return 1U << 4;
	case PERIPHERAL_WDT:
		return 1U << 3;
	default:
		return 0;
	}
}

void pwr_reboot(void)
{
	PMU->CFG |= SWRST;
}

uint32_t pwr_get_reboot_source(void)
{
	return PMU->RSSR;
}

void pwr_clear_reboot_source(uint32_t bitmask)
{
	PMU->RSSR &= ~bitmask;
}

void pwr_set_mode(pwr_mode_t sleep_mode)
{
	uint32_t bit = 0;
	PMU->MR &= ~(PMUMODE_MASK | VDCLP_MASK);

	switch (sleep_mode) {
	case PWR_MODE_RUN:
		bit = 0;
		break;
	case PWR_MODE_SLEEP:
		bit = (1U << PMUMODE_POS);
		break;
	case PWR_MODE_DEEP_SLEEP:
		bit = (2U << PMUMODE_POS);
		break;
	case PWR_MODE_BLACKOUT:
		bit = (2U << PMUMODE_POS) | (1U << VDCLP_POS);
		break;
	default:
		break;
	}

	PMU->MR |= bit;
}

void pwr_set_wakeup_source(peripheral_t peri)
{
	PMU->WSER |= get_wakeup_bitmask_from_enum(peri);
}

void pwr_clear_wakeup_source(peripheral_t peri)
{
	PMU->WSER &= ~get_wakeup_bitmask_from_enum(peri);
}

uint32_t pwr_get_wakeup_source(void)
{
	return PMU->WSSR;
}

void pwr_enable_peripheral(peripheral_t peri)
{
	PMU->PER |= get_activation_bitmask_from_enum(peri);
}

void pwr_disable_peripheral(peripheral_t peri)
{
	PMU->PER &= ~get_activation_bitmask_from_enum(peri);
}

void pwr_reset(void)
{
	PMU->MR = 0;
	PMU->CFG = 0;
	PMU->WSER = 0;
	PMU->RSER = 0x69;
	PMU->RSSR = 0;
	PMU->PRER = 0xB3FF7FF8;
	PMU->PER = 0xB3FF7FF8;
	PMU->PCCR = 0x118;
	PMU->CCR = 0x80;
	PMU->CMR = 0;
	PMU->MCMR = 0;
	PMU->BCCR = 0;
	PMU->PCSR = 0;
	PMU->COR = 0;
	PMU->PLLCON = 0;
	PMU->LVDCON = 0x8800;
	PMU->VDCCON = 0xFF;
	PMU->IOSC16TRIM = 0x228B;
	PMU->EOSCCON = 1;
	PMU->EXTMODER = 0;
}
