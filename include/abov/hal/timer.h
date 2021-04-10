#ifndef ABOV_TIMER_HAL_H
#define ABOV_TIMER_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include "abov/ll/timer.h"

/** Timer configuration */
struct timer_cfg {
	peripheral_t timer;
	timer_mode_t mode;
	uint32_t frequency;
	timer_event_t irq;
	int irq_priority;
};

/***/
bool timer_init(peripheral_t timer, const struct timer_cfg *cfg);
/***/
void timer_deinit(peripheral_t timer);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_HAL_H */
