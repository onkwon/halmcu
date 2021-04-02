#ifndef ABOV_NVIC_H
#define ABOV_NVIC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

typedef void (*nvic_vector_t)(void);

void ISR_reset(void);
void default_fallback_handler(void);
void default_irq_handler(void);

void nvic_enable_irq(uint32_t nirq);
void nvic_disable_irq(uint32_t nirq);
void nvic_set_priority(int32_t nirq, uint32_t priority);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_NVIC_H */
