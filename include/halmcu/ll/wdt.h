#ifndef HALMCU_WDT_LL_H
#define HALMCU_WDT_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "halmcu/periph/wdt.h"

/**
 * @brief Reset WDT unit
 *
 * This function makes WDT unit the reset state.
 */
void wdt_ll_reset(void);
/** Get the current watchdog count */
uint32_t wdt_ll_get_count(void);
void wdt_ll_set_reload_ms(uint32_t period_ms);

/**
 * @brief Set watchdog interrupt
 *
 * The default behavior is to reset the system if not interrupt set
 *
 * @param[in] enable enables interrupt when true. disables when false
 */
void wdt_ll_set_interrupt(bool enable);
bool wdt_ll_is_event_raised(void);

void wdt_ll_set_prescaler(uint32_t div_factor);
uint32_t wdt_ll_get_prescaler(void);
void wdt_ll_set_reload(uint32_t timeout);
uint32_t wdt_ll_get_reload(void);
void wdt_ll_feed(void);
void wdt_ll_start(void);
void wdt_ll_stop(void);
void wdt_ll_set_debug_stop_mode(bool enable);

#include "halmcu/periph/clk.h"

void wdt_ll_set_clock_source(clk_source_t clk);
clk_source_t wdt_ll_get_clock_source(void);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_WDT_LL_H */
