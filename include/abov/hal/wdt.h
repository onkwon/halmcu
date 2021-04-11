#ifndef ABOV_WDT_HAL_H
#define ABOV_WDT_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/wdt.h"

/** Enable Watchdog */
void wdt_enable(void);
/** Disable Watchdog */
void wdt_disable(void);

/***/
void wdt_set_ms(uint32_t period_ms);
/***/
void wdt_feed(void);

/***/
uint32_t wdt_get_clock_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_HAL_H */
