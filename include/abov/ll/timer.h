#ifndef ABOV_TIMER_LL_H
#define ABOV_TIMER_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/hal/peripheral.h"

/** Timer mode type */
typedef enum {
	/***/
	TIMER_MODE_NORMAL,
	/***/
	TIMER_MODE_PWM,
	/***/
	TIMER_MODE_ONESHOT,
	/***/
	TIMER_MODE_INPUT_CAPTURE,
} timer_mode_t;

/** Timer IRQ type */
typedef enum {
	/***/
	TIMER_EVENT_OVERFLOW			= (1U << 0),
	/***/
	TIMER_EVENT_UNDERFLOW			= (1U << 1),
	/** Capture/Compare Channel 0 interrupt */
	TIMER_EVENT_CC_0			= (1U << 2),
	/** Capture/Compare Channel 1 interrupt */
	TIMER_EVENT_CC_1			= (1U << 3),
	/** Capture/Compare Channel 2 interrupt */
	TIMER_EVENT_CC_2			= (1U << 4),
	/** Capture/Compare Channel 3 interrupt */
	TIMER_EVENT_CC_3			= (1U << 5),
} timer_event_t;

/** Timer edge type */
typedef enum {
	/***/
	TIMER_RISING_EDGE,
	/***/
	TIMER_FALLING_EDGE,
} timer_edge_t;

/**
 * Reset Timer interface
 *
 * This function makes the given Timer the reset default state.
 *
 * :param peri: a enum of :c:type:`peripheral_t`
 */
void timer_reset(peripheral_t peri);
/***/
void timer_set_mode(peripheral_t peri, timer_mode_t mode);
/***/
void timer_start(peripheral_t peri);
/***/
void timer_stop(peripheral_t peri);
/***/
void timer_set_prescaler(peripheral_t peri, uint32_t div_factor);
/***/
void timer_set_divider(peripheral_t peri, uint32_t div_factor);
/***/
void timer_enable_irq(peripheral_t peri, timer_event_t events);
/***/
void timer_disable_irq(peripheral_t peri, timer_event_t events);
/***/
void timer_clear_event(peripheral_t peri, timer_event_t events);
/***/
uint32_t timer_get_event(peripheral_t peri);
/***/
void timer_set_reload(peripheral_t peri, uint32_t value);
/***/
void timer_set_counter(peripheral_t peri, uint32_t value);
/***/
uint32_t timer_get_reload(peripheral_t peri);
/***/
uint32_t timer_get_counter(peripheral_t peri);
/**
 * Set Capture/Compare register
 *
 * :param peri: a enum of :c:type:`peripheral_t`
 * :param cc: a number of capture/compare channel
 * :param value: to be written to the capture/compare register
 */
void timer_set_cc(peripheral_t peri, uint32_t cc, uint32_t value);
/**
 * Get Capture/Compare register
 *
 * :param peri: a enum of :c:type:`peripheral_t`
 * :param cc: a number of capture/compare channel
 */
uint32_t timer_get_cc(peripheral_t peri, uint32_t cc);
/***/
void timer_set_polarity(peripheral_t peri, uint32_t level);
/***/
void timer_set_edge(peripheral_t peri, timer_edge_t edge);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
