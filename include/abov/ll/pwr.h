#ifndef ABOV_PWR_LL_H
#define ABOV_PWR_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/** Power mode enumeration */
typedef enum {
	/** Normal running mode */
	LL_POWER_MODE_RUN,
	/** CPU clock gets stopped while core peripherals are kept running */
	LL_POWER_MODE_SLEEP,
	/** Not only CPU clock but also most of peripheral clocks get stopped.
	 * External clock sources are also get off */
	LL_POWER_MODE_DEEP_SLEEP,
	/** RTC is the only one running. SRAM and peripheral registers are not
	 * preserved */
	LL_POWER_MODE_BLACKOUT,
} pwr_mode_t;

enum pwr_ll_peripheral {
	LL_PERIPHERAL_BASE		= 0,
	LL_PERIPHERAL_DEBUG		= LL_PERIPHERAL_BASE + 0x0000,
	LL_PERIPHERAL_POWER		= LL_PERIPHERAL_BASE + 0x0100,
	LL_PERIPHERAL_WDT		= LL_PERIPHERAL_BASE + 0x0200,
	LL_PERIPHERAL_GPIO		= LL_PERIPHERAL_BASE + 0x0300,
	LL_PERIPHERAL_AFIO		= LL_PERIPHERAL_BASE + 0x0400,
	LL_PERIPHERAL_REMAP		= LL_PERIPHERAL_BASE + 0x0500,
	LL_PERIPHERAL_TIMER		= LL_PERIPHERAL_BASE + 0x0600,
	LL_PERIPHERAL_PWM		= LL_PERIPHERAL_BASE + 0x0700,
	LL_PERIPHERAL_DMA		= LL_PERIPHERAL_BASE + 0x0800,
	LL_PERIPHERAL_ADC		= LL_PERIPHERAL_BASE + 0x0900,
	LL_PERIPHERAL_DAC		= LL_PERIPHERAL_BASE + 0x0A00,
	LL_PERIPHERAL_UART		= LL_PERIPHERAL_BASE + 0x0B00,
	LL_PERIPHERAL_SPI		= LL_PERIPHERAL_BASE + 0x0C00,
	LL_PERIPHERAL_I2C		= LL_PERIPHERAL_BASE + 0x0D00,
	LL_PERIPHERAL_CAN		= LL_PERIPHERAL_BASE + 0x0E00,
	LL_PERIPHERAL_USB		= LL_PERIPHERAL_BASE + 0x0F00,
	LL_PERIPHERAL_CRC		= LL_PERIPHERAL_BASE + 0x1000,
	LL_PERIPHERAL_JTAG		= LL_PERIPHERAL_BASE + 0x1100,
	LL_PERIPHERAL_PMC		= LL_PERIPHERAL_BASE + 0x1200,
	LL_PERIPHERAL_FRT		= LL_PERIPHERAL_BASE + 0x1300,
	LL_PERIPHERAL_USART		= LL_PERIPHERAL_BASE + 0x1400,
	LL_PERIPHERAL_MXFAIL		= LL_PERIPHERAL_BASE + 0x1500,
	LL_PERIPHERAL_LVD		= LL_PERIPHERAL_BASE + 0x1600,
};

/** Software system reset */
void pwr_ll_reboot(void);
/** Get reboot source */
uint32_t pwr_ll_get_reboot_source(void);
/** Clear reboot source */
void pwr_ll_clear_reboot_source(uint32_t bitmask);

/** Set power(sleep) mode */
void pwr_ll_set_mode(pwr_mode_t sleep_mode);

/** Set wakeup source */
void pwr_ll_set_wakeup_source(uint32_t peri);
/** Clear wakeup source */
void pwr_ll_clear_wakeup_source(uint32_t peri);
/** Get wakeup source */
uint32_t pwr_ll_get_wakeup_source(void);

/** Enable peripheral */
void pwr_ll_enable_peripheral(uint32_t peri);
/** Disable peripheral */
void pwr_ll_disable_peripheral(uint32_t peri);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PWR_LL_H */
