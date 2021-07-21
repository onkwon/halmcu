#ifndef HALMCU_WDT_H
#define HALMCU_WDT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void wdt_set_prescaler(uint32_t div_factor);
uint32_t wdt_get_prescaler(void);
void wdt_set_reload(uint32_t timeout);
uint32_t wdt_get_reload(void);
void wdt_feed(void);
void wdt_start(void);
void wdt_stop(void);
/**
 * @brief Stop watchdog counter clock when core is halted
 *
 * @param[in] enable counter clock stopped when true. Otherwise continues
 */
void wdt_set_debug_stop_mode(bool enable);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_WDT_H */
