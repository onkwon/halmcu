#ifndef ABOV_WDT_HAL_H
#define ABOV_WDT_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * Reset WDT unit
 *
 * This function makes WDT unit the reset state.
 */
void wdt_reset(void);

/***/
void wdt_set_prescaler(uint32_t div_factor);
/***/
uint32_t wdt_get_prescaler(void);
/***/
void wdt_reload(uint32_t timeout);
/***/
uint32_t wdt_get_reload(void);
/***/
void wdt_start(void);
/***/
void wdt_stop(void);
/***/
void wdt_set_debug_hold_mode(bool enable);
/***/
bool wdt_is_event_raised(void);
/**
 * Set watchdog interrupt
 *
 * The default behavior is to reset the system if not interrupt set
 *
 * :param enable: true enables interrupt. false triggers reset
 */
void wdt_set_interrupt(bool enable);
/** Get the current watchdog count */
uint32_t wdt_get_count(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_HAL_H */
