#ifndef HALMCU_ASSERT_H
#define HALMCU_ASSERT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/asm/core.h"

#define assert(exp) \
	do { \
		if (!(exp)) { \
			const uintptr_t *pc = (const uintptr_t *)halmcu_get_pc(); \
			const uintptr_t *lr = (const uintptr_t *)halmcu_get_lr(); \
			halmcu_assertion_failed(pc, lr); \
		} \
	} while (0)

void halmcu_assertion_failed(const uintptr_t *pc, const uintptr_t *lr);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_ASSERT_H */
