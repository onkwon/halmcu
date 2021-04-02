#ifndef ABOV_WDT_LL_H
#define ABOV_WDT_LL_H

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

void wdt_ll_set_prescaler(uint32_t div_factor);
void wdt_ll_reload(uint32_t timeout);
void wdt_ll_start(void);
void wdt_ll_stop(void);
void wdt_ll_set_debug_hold_mode(bool enable);
bool wdt_ll_is_event_raised(void);
void wdt_ll_set_interrupt(bool enable);
uint32_t wdt_ll_get_count(void);
uint32_t wdt_ll_get_timeout(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_LL_H */
