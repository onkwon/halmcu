#ifndef ABOV_ATOMIC_H
#define ABOV_ATOMIC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define ldrex(addr) __extension__ ({ \
		uintptr_t res = 0; \
		__asm__ __volatile__("ldrex %0, [%1]" \
				: "=r"(res) \
				: "r"(addr) \
				: "cc", "memory"); \
				res; \
})

/* return 0 on success */
#define strex(value, addr) __extension__ ({ \
		uintptr_t res = 0; \
		__asm__ __volatile__("strex %0, %2, [%1]" \
				: "=&r"(res) \
				: "r"(addr), "r"(value) \
				: "cc", "memory"); \
				res; \
})

/** Load linked */
static inline uintptr_t atomic_ll(const void *p)
{
	return ldrex(p);
}

/**
 * Store conditional
 *
 * @param p Pointer to data
 * @param newval Value to store
 * @return 0 on success
 */
static inline uintptr_t atomic_sc(uintptr_t newval, void *p)
{
	return strex(newval, p);
}

/**
 * Fetch and add
 *
 * @param p Pointer to data
 * @param inc Increment to be added
 * @return Original value at @param p
 */
static inline int atomic_faa(int *p, int inc)
{
	int val;

	do {
		val = (int)ldrex(p);
	} while (strex((uintptr_t)(val + inc), p) != 0);

	return val;
}

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ATOMIC_H */
