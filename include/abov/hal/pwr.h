#ifndef ABOV_PWR_HAL_H
#define ABOV_PWR_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/hal/peripheral.h"

/** Power mode enumeration */
typedef enum {
	/** Normal running mode */
	PWR_MODE_RUN,
	/** CPU clock gets stopped while core peripherals are kept running */
	PWR_MODE_SLEEP,
	/** Not only CPU clock but also most of peripheral clocks get stopped.
	 * External clock sources are also get off */
	PWR_MODE_DEEP_SLEEP,
	/** RTC is the only one running. SRAM and peripheral registers are not
	 * preserved */
	PWR_MODE_BLACKOUT,
} pwr_mode_t;

/** Software system reset */
void pwr_reboot(void);
/** Get reboot source */
uint32_t pwr_get_reboot_source(void);
/** Clear reboot source */
void pwr_clear_reboot_source(uint32_t bitmask);

/** Set power(sleep) mode */
void pwr_set_mode(pwr_mode_t sleep_mode);

/** Set wakeup source */
void pwr_set_wakeup_source(uint32_t peri);
/** Clear wakeup source */
void pwr_clear_wakeup_source(uint32_t peri);
/** Get wakeup source */
uint32_t pwr_get_wakeup_source(void);

/** Enable peripheral */
void pwr_enable_peripheral(uint32_t peri);
/** Disable peripheral */
void pwr_disable_peripheral(uint32_t peri);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PWR_HAL_H */
