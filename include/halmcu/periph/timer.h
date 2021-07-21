#ifndef HALMCU_TIMER_H
#define HALMCU_TIMER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "halmcu/periph/periph.h"

/** Timer mode type */
typedef enum {
	TIMER_MODE_NORMAL,
	TIMER_MODE_PWM,
	TIMER_MODE_ONESHOT,
	TIMER_MODE_CAPTURE,
} timer_mode_t;

/** Timer pin type */
typedef enum {
	TIMER_CC_0,
	TIMER_CC_1,
	TIMER_CC_2,
	TIMER_CC_3,
	TIMER_CC_4,
	TIMER_CC_1N,
	TIMER_CC_2N,
	TIMER_CC_3N,
	TIMER_CC_4N,
} timer_cc_t;

/** Timer CC mode type */
typedef enum {
	TIMER_CC_MODE_NONE,
	TIMER_CC_MODE_ACTIVE_HIGH,
	TIMER_CC_MODE_ACTIVE_LOW,
	TIMER_CC_MODE_TOGGLE,
	TIMER_CC_MODE_LOW,
	TIMER_CC_MODE_HIGH,
	TIMER_CC_MODE_PWM_ACTIVE_HIGH,
	TIMER_CC_MODE_PWM_ACTIVE_LOW,
} timer_cc_mode_t;

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
 * @brief Set the timer prescaler
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] div_factor prescaler values
 */
void timer_set_prescaler(periph_t peri, uint32_t div_factor);
/**
 * @brief Get the timer prescaler
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 */
uint32_t timer_get_prescaler(periph_t peri);
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
/**
 * @brief Set Capture/Compare register
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel
 * @param[in] value to be written to the capture/compare register
 */
void timer_set_cc(periph_t peri, timer_cc_t cc, uint32_t value);
/**
 * @brief Get Capture/Compare register
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel
 * @return capture/compare value
 */
uint32_t timer_get_cc(periph_t peri, timer_cc_t cc);
/**
 * @brief Enable Capture/Compare pin
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel @ref timer_cc_t
 */
void timer_enable_cc_pin(periph_t peri, timer_cc_t cc);
/**
 * @brief Disable Capture/Compare pin
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel @ref timer_cc_t
 */
void timer_disable_cc_pin(periph_t peri, timer_cc_t cc);
/**
 * @brief Set Capture/Compare pin map
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel @ref timer_cc_t
 * @param[in] value 0 for output, 1 for TI1, 2 for TI2, and 3 for TRC
 */
void timer_set_cc_pin(periph_t peri, timer_cc_t cc, uint32_t value);
/**
 * @brief Set Capture/Compare pin mode
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel @ref timer_cc_t
 * @param[in] mode capture/compare output mode in @ref timer_cc_mode_t
 */
void timer_set_cc_pin_mode(periph_t peri, timer_cc_t cc, timer_cc_mode_t mode);
/**
 * @brief Set Capture/Compare polarity
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] cc a number of capture/compare channel @ref timer_cc_t
 * @param[in] active_high active high on true
 */
void timer_set_cc_pin_polarity(periph_t peri, timer_cc_t cc, bool active_high);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_TIMER_H */
