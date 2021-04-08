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
	TIMER_IRQ_CAPTURE			= (1U << 0),
	TIMER_IRQ_COMPARE			= (1U << 1),
	TIMER_IRQ_OVERFLOW			= (1U << 2),
	TIMER_IRQ_UNDERFLOW			= (1U << 3),
} timer_intr_t;

typedef enum {
	TIMER_CHANNEL_1,
	TIMER_CHANNEL_2,
	TIMER_CHANNEL_3,
	TIMER_CHANNEL_4,
} timer_channel_t;

void timer_set_mode(peripheral_t peri, timer_mode_t mode);
void timer_start(peripheral_t peri);
void timer_stop(peripheral_t peri);
void timer_set_prescaler(peripheral_t peri, uint32_t div_factor);
void timer_set_divider(peripheral_t peri, uint32_t div_factor);
void timer_enable_irq(peripheral_t peri, timer_intr_t events);
void timer_disable_irq(peripheral_t peri, timer_intr_t events);
void timer_clear_event(peripheral_t peri, timer_intr_t events);
void timer_enable_channel_irq(peripheral_t peri, timer_channel_t channel, timer_intr_t events);
void timer_disable_channel_irq(peripheral_t peri, timer_channel_t channel, timer_intr_t events);
void timer_clear_channel_event(peripheral_t peri, timer_channel_t channel, timer_intr_t events);
void timer_set_reload(peripheral_t peri, uint32_t value);
uint32_t timer_get_reload(peripheral_t peri);
uint32_t timer_get_counter(peripheral_t peri);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_LL_H */
