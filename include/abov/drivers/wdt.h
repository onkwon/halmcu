#ifndef ABOV_WDT_H
#define ABOV_WDT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/**
 * Start the Watchdog timer.
 *
 * :param timeout_ms: Watchdog timeout in milliseconds
 * 	(0 < timeout_ms <= :c:func:`wdt_get_max_timeout`).
 * :return: 0 on success, non-zero error code on error.
 */
int wdt_start(uint32_t timeout_ms);
/**
 * Stop the Watchdog timer.
 *
 * :return: 0 on success, non-zero error code on error.
 */
int wdt_stop(void);
/**
 * Refresh the Watchdog timer.
 *
 * This function should be called before the Watchdog times out.
 * Otherwise the system gets reset.
 */
void wdt_feed(void);
/**
 * Get the Watchdog timer timeout interval in milliseconds.
 *
 * :return: Reload value for the Watchdog timer in milliseconds.
 */
uint32_t wdt_get_timeout(void);
/**
 * Get the maximum Watchdog timer timeout interval in milliseconds.
 *
 * :return: Maximum reload value for the Watchdog timer in milliseconds.
 */
uint32_t wdt_get_max_timeout(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_WDT_H */
