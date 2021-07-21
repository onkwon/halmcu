#ifndef HALMCU_WDT_HAL_H
#define HALMCU_WDT_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/wdt.h"

void wdt_enable(void);
void wdt_disable(void);
uint32_t wdt_get_clock_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_WDT_HAL_H */
