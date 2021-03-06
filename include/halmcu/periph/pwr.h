#ifndef LIBMCU_PWR_H
#define LIBMCU_PWR_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "halmcu/periph/periph.h"

/** Power mode enumeration */
typedef enum {
	PWR_MODE_RUN,        /**< Normal running mode */
	PWR_MODE_SLEEP,      /**< CPU clock gets stopped while core peripherals
			       are kept running */
	PWR_MODE_DEEP_SLEEP, /**< Not only CPU clock but also most of peripheral
			       clocks get stopped. External clock sources are
			       also get off */
	PWR_MODE_BLACKOUT,   /**< RTC is the only one running. SRAM and
			       peripheral registers are not preserved */
} pwr_mode_t;

/**
 * @brief Reset PWR unit
 *
 * This function makes PWR unit the reset state.
 */
void pwr_reset(void);

/** Software system reset */
void pwr_reboot(void);
/** Get reboot source */
uint32_t pwr_get_reboot_source(void);
/** Clear reboot source */
void pwr_clear_reboot_source(uint32_t bitmask);

/** Set power(sleep) mode */
void pwr_set_mode(pwr_mode_t sleep_mode);

/** Set wakeup source */
void pwr_set_wakeup_source(periph_t peri);
/** Clear wakeup source */
void pwr_clear_wakeup_source(periph_t peri);
/** Get wakeup source */
uint32_t pwr_get_wakeup_source(void);

/** Enable peripheral */
void pwr_enable_peripheral(periph_t peri);
/** Disable peripheral */
void pwr_disable_peripheral(periph_t peri);

#if defined(__cplusplus)
}
#endif

#endif /* LIBMCU_PWR_H */
