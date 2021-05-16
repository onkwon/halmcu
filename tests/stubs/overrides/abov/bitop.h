#ifndef ABOV_BITOP_H
#define ABOV_BITOP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void bitop_set(volatile uint32_t *reg, uint32_t pos);
void bitop_clear(volatile uint32_t *reg, uint32_t pos);
void bitop_clean_set_with_mask(volatile uint32_t *reg,
		uint32_t pos, uint32_t mask, uint32_t val);
bool is_pwr2(uint32_t val);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_BITOP_H */
