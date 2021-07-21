#ifndef EXTI_H
#define EXTI_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "halmcu/periph/periph.h"

typedef enum {
	EXTI_0,
	EXTI_1,
	EXTI_2,
	EXTI_3,
	EXTI_4,
	EXTI_5,
	EXTI_6,
	EXTI_7,
	EXTI_8,
	EXTI_9,
	EXTI_10,
	EXTI_11,
	EXTI_12,
	EXTI_13,
	EXTI_14,
	EXTI_15,
	EXTI_16,
	EXTI_17,
	EXTI_18,
	EXTI_19,
} exti_t;

void exti_enable_irq(exti_t exti);
void exti_disable_irq(exti_t exti);
void exti_enable_event(exti_t exti);
void exti_disable_event(exti_t exti);
void exti_set_source(periph_t port, uint32_t pin);
void exti_set_rising_trigger(exti_t exti);
void exti_clear_rising_trigger(exti_t exti);
void exti_set_falling_trigger(exti_t exti);
void exti_clear_falling_trigger(exti_t exti);
void exti_raise_irq(exti_t exti);
void exti_clear_event(exti_t exti);
uint32_t exti_get_event(void);

#if defined(__cplusplus)
}
#endif

#endif /* EXTI_H */
