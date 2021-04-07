#ifndef ABOV_SYSTICK_H
#define ABOV_SYSTICK_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

/** Start SysTick */
void systick_start(void);
/** Stop SysTick */
void systick_stop(void);
/** Clear SysTick counter value */
void systick_clear(void);
/** Set SysTick clock frequency */
uint32_t systick_set_frequency(uint32_t hz);
/** Get SysTick clock frequency */
uint32_t systick_get_frequency(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SYSTICK_H */
