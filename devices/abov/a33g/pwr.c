#include "abov/ll/pwr.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define PMUMODE_POS				0
#define VDCLP_POS				2

#define PMUMODE_MASK				(3U << PMUMODE_POS)
#define VDCLP_MASK				(1U << VDCLP_POS)

#define SWRST					1

static uint32_t get_wakeup_bitmask_from_enum(uint32_t x)
{
	switch (x) {
	case PERI_GPIOA: /* fall through */
	case PERI_GPIOB: /* fall through */
	case PERI_GPIOC: /* fall through */
	case PERI_GPIOD: /* fall through */
	case PERI_GPIOE: /* fall through */
	case PERI_GPIOF:
		return 1U << (x - PERI_GPIOA + 5);
	case PERI_FRT:
		return 1U << 4;
	case PERI_WDT:
		return 1U << 3;
	case PERI_MXOSCFAIL:
		return 1U << 1;
	case PERI_LVDFAIL:
		return 1U << 0;
	default:
		return 0;
	}
}

static uint32_t get_activation_bitmask_from_enum(uint32_t x)
{
	switch (x) {
	case PERI_JTAG:
		return 1U << 31;
	case PERI_PMC:
		return 1U << 29;
	case PERI_ADC:
		return 1U << 28;
	case PERI_PWM0: /* fall through */
	case PERI_PWM1: /* fall through */
	case PERI_PWM2: /* fall through */
	case PERI_PWM3: /* fall through */
	case PERI_PWM4: /* fall through */
	case PERI_PWM5: /* fall through */
	case PERI_PWM6: /* fall through */
	case PERI_PWM7:
		return 1U << (((x - PERI_PWM0) >> 2) + 24);
	case PERI_UART0: /* fall through */
	case PERI_UART1: /* fall through */
	case PERI_UART2: /* fall through */
	case PERI_UART3:
		return 1U << (x - PERI_UART0 + 20);
	case PERI_I2C0: /* fall through */
	case PERI_I2C1:
		return 1U << (x - PERI_I2C0 + 18);
	case PERI_SPI0: /* fall through */
	case PERI_SPI1:
		return 1U << (x - PERI_SPI0 + 16);
	case PERI_GPIOA: /* fall through */
	case PERI_GPIOB: /* fall through */
	case PERI_GPIOC: /* fall through */
	case PERI_GPIOD: /* fall through */
	case PERI_GPIOE: /* fall through */
	case PERI_GPIOF:
		return 1U << (x - PERI_GPIOA + 8);
	case PERI_TIMER0: /* fall through */
	case PERI_TIMER1:
		return 1U << 5;
	case PERI_TIMER2: /* fall through */
	case PERI_TIMER3: /* fall through */
	case PERI_TIMER4: /* fall through */
	case PERI_TIMER5:
		return 1U << 6;
	case PERI_TIMER6: /* fall through */
	case PERI_TIMER7: /* fall through */
	case PERI_TIMER8: /* fall through */
	case PERI_TIMER9:
		return 1U << 7;
	case PERI_FRT:
		return 1U << 4;
	case PERI_WDT:
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
