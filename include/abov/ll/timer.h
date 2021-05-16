#ifndef ABOV_TIMER_LL_H
#define ABOV_TIMER_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/periph/timer.h"

/**
 * @brief Reset the timer interface
 *
 * This function makes the given timer in the reset default state.
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 */
void timer_ll_reset(periph_t peri);
/**
 * @brief Set the timer mode
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] mode one of @ref timer_mode_t
 */
void timer_ll_set_mode(periph_t peri, timer_mode_t mode);
/**
 * @brief Enable interrupts on events for a timer
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] events to be enabled
 */
void timer_ll_enable_irq(periph_t peri, timer_event_t events);
/**
 * @brief Disable interrupts on events for a timer
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] events to be disabled
 */
void timer_ll_disable_irq(periph_t peri, timer_event_t events);
/**
 * @brief Set the timer clock divider
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] div_factor clock divider
 */
void timer_ll_set_clock_divider(periph_t peri, uint32_t div_factor);
/**
 * @brief Set the timer counter
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] value to be written
 */
void timer_ll_set_counter(periph_t peri, uint32_t value);
/**
 * @brief Get the timer counter
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @return timer counter
 */
uint32_t timer_ll_get_counter(periph_t peri);
void timer_ll_set_prescaler(periph_t peri, uint32_t div_factor);
uint32_t timer_ll_get_prescaler(periph_t peri);
void timer_ll_set_reload(periph_t peri, uint32_t value);
uint32_t timer_ll_get_reload(periph_t peri);
void timer_ll_clear_event(periph_t peri, timer_event_t events);
timer_event_t timer_ll_get_event(periph_t peri);
void timer_ll_start(periph_t peri);
void timer_ll_stop(periph_t peri);
uint32_t timer_ll_get_frequency(periph_t peri, uint32_t tclk);

void timer_ll_set_cc(periph_t peri, timer_cc_t cc, uint32_t value);
uint32_t timer_ll_get_cc(periph_t peri, timer_cc_t cc);

void timer_ll_enable_cc_pin(periph_t peri, timer_cc_t cc);
void timer_ll_disable_cc_pin(periph_t peri, timer_cc_t cc);
void timer_ll_set_cc_pin(periph_t peri, timer_cc_t cc, uint32_t value);
void timer_ll_set_cc_pin_mode(periph_t peri, timer_cc_t cc, timer_cc_mode_t mode);
void timer_ll_set_cc_pin_polarity(periph_t peri, timer_cc_t cc, bool active_high);

void timer_ll_enable_cc_preload(periph_t peri, timer_cc_t cc);
void timer_ll_disable_cc_preload(periph_t peri, timer_cc_t cc);
void timer_ll_enable_cc_fastmode(periph_t peri, timer_cc_t cc);
void timer_ll_disable_cc_fastmode(periph_t peri, timer_cc_t cc);
void timer_ll_set_cc_prescaler(periph_t peri, timer_cc_t cc, uint32_t value);
void timer_ll_set_cc_filter(periph_t peri, timer_cc_t cc, uint32_t value);

void timer_ll_set_counter_direction(periph_t peri, timer_direction_t dir);
void timer_ll_set_counter_alignment_mode(periph_t peri, uint32_t align);

void timer_ll_set_slave_mode(periph_t peri, uint32_t value);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
