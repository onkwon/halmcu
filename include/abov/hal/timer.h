#ifndef ABOV_TIMER_HAL_H
#define ABOV_TIMER_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include "abov/ll/timer.h"

/** Timer configuration */
struct timer_cfg {
	timer_mode_t mode;
	uint32_t frequency;
	timer_event_t irq;
	int irq_priority;
	void (*set_clock_source)(void);
};

/**
 * Initialize the timer
 *
 * :param timer: a peripheral enumerated in :c:type:`peripheral_t`
 * :param cfg: pointer to the structure with the initial configuration
 * :return: true on success
 */
bool timer_init(peripheral_t timer, const struct timer_cfg *cfg);
/**
 * Deinitialize the timer
 *
 * :param timer: a peripheral enumerated in :c:type:`peripheral_t`
 */
void timer_deinit(peripheral_t timer);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_HAL_H */
