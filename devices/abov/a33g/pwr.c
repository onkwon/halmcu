#include "abov/hal/pwr.h"
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
	switch (x) {
	case PERIPHERAL_GPIOA:
		return 1U << 5;
	case PERIPHERAL_GPIOB:
		return 1U << 6;
	case PERIPHERAL_GPIOC:
		return 1U << 7;
	case PERIPHERAL_GPIOD:
		return 1U << 8;
	case PERIPHERAL_GPIOE:
		return 1U << 9;
	case PERIPHERAL_GPIOF:
		return 1U << 10;
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
	switch (x) {
	case PERIPHERAL_JTAG:
		return 1U << 31;
	case PERIPHERAL_PMC:
		return 1U << 29;
	case PERIPHERAL_ADC:
		return 1U << 28;
	case PERIPHERAL_PWM4:
	case PERIPHERAL_PWM5:
	case PERIPHERAL_PWM6:
	case PERIPHERAL_PWM7:
		return 1U << 25;
	case PERIPHERAL_PWM0:
	case PERIPHERAL_PWM1:
	case PERIPHERAL_PWM2:
	case PERIPHERAL_PWM3:
		return 1U << 24;
	case PERIPHERAL_UART3:
		return 1U << 23;
	case PERIPHERAL_UART2:
		return 1U << 22;
	case PERIPHERAL_UART1:
		return 1U << 21;
	case PERIPHERAL_UART0:
		return 1U << 20;
	case PERIPHERAL_I2C1:
		return 1U << 19;
	case PERIPHERAL_I2C0:
		return 1U << 18;
	case PERIPHERAL_SPI1:
		return 1U << 17;
	case PERIPHERAL_SPI0:
		return 1U << 16;
	case PERIPHERAL_CRC:
		return 1U << 14;
	case PERIPHERAL_GPIOF:
		return 1U << 13;
	case PERIPHERAL_GPIOE:
		return 1U << 12;
	case PERIPHERAL_GPIOD:
		return 1U << 11;
	case PERIPHERAL_GPIOC:
		return 1U << 10;
	case PERIPHERAL_GPIOB:
		return 1U << 9;
	case PERIPHERAL_GPIOA:
		return 1U << 8;
	case PERIPHERAL_TIMER9:
	case PERIPHERAL_TIMER8:
	case PERIPHERAL_TIMER7:
	case PERIPHERAL_TIMER6:
		return 1U << 7;
	case PERIPHERAL_TIMER5:
	case PERIPHERAL_TIMER4:
	case PERIPHERAL_TIMER3:
	case PERIPHERAL_TIMER2:
		return 1U << 6;
	case PERIPHERAL_TIMER1:
	case PERIPHERAL_TIMER0:
		return 1U << 5;
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
