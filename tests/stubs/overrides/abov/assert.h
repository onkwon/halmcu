#ifndef ASSERT_OVERRIDE_H
#define ASSERT_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#undef assert
#define assert(exp)		if (!(exp)) abov_assertion_failed(0, 0)

void abov_assertion_failed(const uintptr_t *pc, const uintptr_t *lr);

#if defined(__cplusplus)
}
#endif

#endif /* ASSERT_OVERRIDE_H */
