#ifndef HALMCU_TIMER_HAL_H
#define HALMCU_TIMER_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include "halmcu/periph/timer.h"
#include "halmcu/ll/timer.h"

/** Timer configuration */
struct timer_cfg {
	timer_mode_t mode;
	uint32_t frequency;
	timer_event_t irq;
	int irq_priority;
	void (*set_clock_source)(void);
};

/**
 * @brief Initialize the timer
 *
 * @param[in] timer a peripheral enumerated in @ref periph_t
 * @param[in] cfg pointer to the structure with the initial configuration
 * @return true on success
 */
bool timer_init(periph_t timer, const struct timer_cfg *cfg);
/**
 * @brief Deinitialize the timer
 *
 * @param[in] timer a peripheral enumerated in @ref periph_t
 */
void timer_deinit(periph_t timer);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_TIMER_HAL_H */
