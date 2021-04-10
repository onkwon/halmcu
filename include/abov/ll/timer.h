#ifndef ABOV_TIMER_LL_H
#define ABOV_TIMER_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/peripheral.h"

/** Timer mode enumeration */
typedef enum {
	/***/
	TIMER_MODE_NORMAL,
	/***/
	TIMER_MODE_PWM,
	/***/
	TIMER_MODE_ONESHOT,
	/***/
	TIMER_MODE_CAPTURE,
} timer_mode_t;

/** Timer IRQ enumeration */
typedef enum {
	/***/
	TIMER_EVENT_NONE			= 0,
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

/** Timer edge enumeration */
typedef enum {
	/***/
	TIMER_RISING_EDGE,
	/***/
	TIMER_FALLING_EDGE,
} timer_edge_t;

/**
 * Reset the timer interface
 *
 * This function makes the given timer in the reset default state.
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 */
void timer_reset(peripheral_t peri);
/**
 * Set the timer mode
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param mode: :c:type:`timer_mode_t`
 */
void timer_set_mode(peripheral_t peri, timer_mode_t mode);
/**
 * Start the timer
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 */
void timer_start(peripheral_t peri);
/**
 * Stop the timer
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 */
void timer_stop(peripheral_t peri);
/**
 * Set the timer prescaler
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param div_factor: prescaler values
 */
void timer_set_prescaler(peripheral_t peri, uint32_t div_factor);
/**
 * Set the timer clock divider
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param div_factor: clock divider
 */
void timer_set_clock_divider(peripheral_t peri, uint32_t div_factor);
/**
 * Enable interrupts on events for a timer
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param events: events to be enabled
 */
void timer_enable_irq(peripheral_t peri, timer_event_t events);
/**
 * Disable interrupts on events for a timer
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param events: events to be disabled
 */
void timer_disable_irq(peripheral_t peri, timer_event_t events);
/**
 * Clear event flags
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param events: events to be cleared
 */
void timer_clear_event(peripheral_t peri, timer_event_t events);
/**
 * Read event flags
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 */
timer_event_t timer_get_event(peripheral_t peri);
/**
 * Set Capture/Compare register
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param cc: a number of capture/compare channel
 * :param value: to be written to the capture/compare register
 */
void timer_set_cc(peripheral_t peri, uint32_t cc, uint32_t value);
/**
 * Get Capture/Compare register
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param cc: a number of capture/compare channel
 * :return: capture/compare value
 */
uint32_t timer_get_cc(peripheral_t peri, uint32_t cc);
/**
 * Set the timer counter
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param value: to be written
 */
void timer_set_counter(peripheral_t peri, uint32_t value);
/**
 * Get the timer counter
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :return: timer counter
 */
uint32_t timer_get_counter(peripheral_t peri);
/**
 * Set the timer period
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :param value: to be written
 */
void timer_set_reload(peripheral_t peri, uint32_t value);
/**
 * Get the timer period
 *
 * :param peri: a peripheral enumerated in :c:type:`peripheral_t`
 * :return: timer period
 */
uint32_t timer_get_reload(peripheral_t peri);
/***/
void timer_set_polarity(peripheral_t peri, uint32_t level);
/***/
void timer_set_edge(peripheral_t peri, timer_edge_t edge);
/***/
uint32_t timer_get_frequency(peripheral_t peri, uint32_t tclk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
