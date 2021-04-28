#ifndef ABOV_ASSERT_H
#define ABOV_ASSERT_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/asm/core.h"

#if defined(UNITTEST)
#define assert(exp)
#else
#define assert(exp) \
	do { \
		if (!(exp)) { \
			const uintptr_t *pc = (const uintptr_t *)abov_get_pc(); \
			const uintptr_t *lr = (const uintptr_t *)abov_get_lr(); \
			abov_assertion_failed(pc, lr); \
		} \
	} while (0)
#endif

void abov_assertion_failed(const uintptr_t *pc, const uintptr_t *lr);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ASSERT_H */
