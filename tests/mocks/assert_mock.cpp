#include "CppUTestExt/MockSupport.h"

#include "abov/assert.h"
#include "abov/compiler.h"

void abov_assertion_failed(const uintptr_t *pc, const uintptr_t *lr) {
	unused(pc);
	unused(lr);
	mock().actualCall(__func__);
}
