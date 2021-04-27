#ifndef ABOV_BITOP_H
#define ABOV_BITOP_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

static inline void bitop_set(volatile uint32_t *reg, uint32_t pos)
{
	*reg |= 1U << pos;
}

static inline void bitop_clear(volatile uint32_t *reg, uint32_t pos)
{
	*reg &= ~(1U << pos);
}

static inline void bitop_clean_set_with_mask(volatile uint32_t *reg,
		uint32_t pos, uint32_t mask, uint32_t val)
{
	uint32_t t = *reg;
	t &= ~(mask << pos);
	t |= (val << pos) & (mask << pos);
	*reg = t;
}

static inline bool is_pwr2(uint32_t val)
{
	return val && !(val & (val - 1));
}

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_BITOP_H */
