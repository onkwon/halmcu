#ifndef ABOV_WDT_LL_H
#define ABOV_WDT_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "abov/periph/wdt.h"

/**
 * @brief Reset WDT unit
 *
 * This function makes WDT unit the reset state.
 */
void wdt_reset(void);
/** Get the current watchdog count */
uint32_t wdt_get_count(void);

/**
 * @brief Set watchdog interrupt
 *
 * The default behavior is to reset the system if not interrupt set
 *
 * @param[in] enable enables interrupt when true. disables when false
 */
void wdt_set_interrupt(bool enable);
bool wdt_is_event_raised(void);

#include "abov/ll/clk.h"

void wdt_set_clock_source(clk_source_t clk);
clk_source_t wdt_get_clock_source(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_LL_H */
