#ifndef ABOV_WDT_HAL_H
#define ABOV_WDT_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/wdt.h"

void wdt_enable(void);
void wdt_disable(void);
uint32_t wdt_get_clock_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_HAL_H */
