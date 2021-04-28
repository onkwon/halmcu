#include "CppUTestExt/MockSupport.h"

#include "abov/assert.h"

void abov_assertion_failed(const uintptr_t *pc, const uintptr_t *lr) {
	mock().actualCall(__func__);
}
