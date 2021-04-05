#ifndef ABOV_WDT_H
#define ABOV_WDT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/hal/wdt.h"
#include "abov/hal/clk.h"

/** Enable Watchdog */
void wdt_enable(void);
/** Disable Watchdog */
void wdt_disable(void);

void wdt_set(uint32_t period_ms);
void wdt_feed(void);

void wdt_set_clock_source(clk_source_t clk);
clk_source_t wdt_get_clock_source(void);
uint32_t wdt_get_clock_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_H */
