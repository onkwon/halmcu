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
 * @brief Set the timer prescaler
 *
 * @param[in] peri a peripheral enumerated in @ref periph_t
 * @param[in] div_factor prescaler values
 */
void timer_ll_set_prescaler(periph_t peri, uint32_t div_factor);
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
void timer_ll_set_reload(periph_t peri, uint32_t value);
uint32_t timer_ll_get_reload(periph_t peri);
void timer_ll_clear_event(periph_t peri, timer_event_t events);
timer_event_t timer_ll_get_event(periph_t peri);
void timer_ll_start(periph_t peri);
void timer_ll_stop(periph_t peri);
void timer_ll_set_cc(periph_t peri, uint32_t cc, uint32_t value);
uint32_t timer_ll_get_cc(periph_t peri, uint32_t cc);
uint32_t timer_ll_get_frequency(periph_t peri, uint32_t tclk);
void timer_ll_set_edge(periph_t peri, timer_edge_t edge);
void timer_ll_set_polarity(periph_t peri, uint32_t level);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
