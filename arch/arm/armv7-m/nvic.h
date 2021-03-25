#ifndef ABOV_NVIC_H
#define ABOV_NVIC_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef void (*nvic_vector_t)(void);

void ISR_reset(void);
void default_fallback_handler(void);
void default_irq_handler(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_NVIC_H */
