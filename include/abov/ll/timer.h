#ifndef ABOV_TIMER_LL_H
#define ABOV_TIMER_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/hal/peripheral.h"

typedef enum {
	TIMER_MODE_NORMAL,
	TIMER_MODE_PWM,
	TIMER_MODE_ONESHOT,
	TIMER_MODE_INPUT_CAPTURE,
} timer_mode_t;

typedef enum {
	TIMER_IRQ_OVERFLOW			= (1U << 0),
	TIMER_IRQ_UNDERFLOW			= (1U << 1),
	TIMER_IRQ_COMPARE_0			= (1U << 2),
	TIMER_IRQ_COMPARE_1			= (1U << 3),
	TIMER_IRQ_COMPARE_2			= (1U << 4),
	TIMER_IRQ_COMPARE_3			= (1U << 5),
} timer_intr_t;

void timer_reset(peripheral_t peri);
void timer_set_mode(peripheral_t peri, timer_mode_t mode);
void timer_start(peripheral_t peri);
void timer_stop(peripheral_t peri);
void timer_set_prescaler(peripheral_t peri, uint32_t div_factor);
void timer_set_divider(peripheral_t peri, uint32_t div_factor);
void timer_enable_irq(peripheral_t peri, timer_intr_t events);
void timer_disable_irq(peripheral_t peri, timer_intr_t events);
void timer_clear_event(peripheral_t peri, timer_intr_t events);
uint32_t timer_get_event(peripheral_t peri);
void timer_set_reload(peripheral_t peri, uint32_t value);
void timer_set_counter(peripheral_t peri, uint32_t value);
uint32_t timer_get_reload(peripheral_t peri);
uint32_t timer_get_counter(peripheral_t peri);
void timer_set_compare(peripheral_t peri, uint32_t ncompare, uint32_t value);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
