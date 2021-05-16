#ifndef ABOV_TIMER_H
#define ABOV_TIMER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/periph/periph.h"

/** Timer mode type */
typedef enum {
	TIMER_MODE_NORMAL,
	TIMER_MODE_PWM,
	TIMER_MODE_ONESHOT,
	TIMER_MODE_CAPTURE,
} timer_mode_t;

/** Timer event type */
typedef enum {
	TIMER_EVENT_NONE			= 0,
	TIMER_EVENT_OVERFLOW			= (1U << 0),
	TIMER_EVENT_UNDERFLOW			= (1U << 1),
	/** Capture/Compare Channel interrupt */
	TIMER_EVENT_CC_0			= (1U << 2),
	TIMER_EVENT_CC_1			= (1U << 3),
	TIMER_EVENT_CC_2			= (1U << 4),
	TIMER_EVENT_CC_3			= (1U << 5),
	TIMER_EVENT_CC_4			= (1U << 6),
	TIMER_EVENT_UPDATE			= (1U << 7),
} timer_event_t;

/** Timer edge type */
typedef enum {
	TIMER_RISING_EDGE,
	TIMER_FALLING_EDGE,
} timer_edge_t;

typedef enum {
	TIMER_DIRECTION_UP,
	TIMER_DIRECTION_DOWN,
} timer_direction_t;

/**
 * @brief Start the timer
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 */
void timer_start(periph_t peri);
/**
 * @brief Stop the timer
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 */
void timer_stop(periph_t peri);
/**
 * @brief Clear event flags
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] events to be cleared
 */
void timer_clear_event(periph_t peri, timer_event_t events);
/**
 * @brief Read event flags
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @return events
 */
timer_event_t timer_get_event(periph_t peri);
/**
 * @brief Set Capture/Compare register
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel
 * @param[in] value to be written to the capture/compare register
 */
void timer_set_cc(periph_t peri, uint32_t cc, uint32_t value);
/**
 * @brief Get Capture/Compare register
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel
 * @return capture/compare value
 */
uint32_t timer_get_cc(periph_t peri, uint32_t cc);
/**
 * @brief Set the timer period
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] value to be written
 */
void timer_set_reload(periph_t peri, uint32_t value);
/**
 * @brief Get the timer period
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @return timer period
 */
uint32_t timer_get_reload(periph_t peri);
void timer_set_edge(periph_t peri, timer_edge_t edge);
void timer_set_polarity(periph_t peri, uint32_t level);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_H */
