#include "CppUTestExt/MockSupport.h"

#include "abov/bitop.h"
#include "abov/compiler.h"

void bitop_set(volatile uint32_t *reg, uint32_t pos) {
	mock().actualCall(__func__)
		.withPointerParameter("reg", (volatile void *)reg)
		.withParameter("pos", pos);

	*reg |= 1U << pos;
}

void bitop_clear(volatile uint32_t *reg, uint32_t pos) {
	mock().actualCall(__func__)
		.withPointerParameter("reg", (volatile void *)reg)
		.withParameter("pos", pos);

	*reg &= ~(1U << pos);
}

void bitop_clean_set_with_mask(volatile uint32_t *reg,
		uint32_t pos, uint32_t mask, uint32_t val) {
	mock().actualCall(__func__)
		.withPointerParameter("reg", (volatile void *)reg)
		.withParameter("pos", pos)
		.withParameter("mask", mask)
		.withParameter("val", val);

	uint32_t t = *reg;
	t &= ~(mask << pos);
	t |= (val << pos) & (mask << pos);
	*reg = t;
}

bool is_pwr2(uint32_t val) {
	unused(val);
	return mock().actualCall(__func__).returnBoolValueOrDefault(false);
}
