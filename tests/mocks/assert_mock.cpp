#include "CppUTestExt/MockSupport.h"

#include "halmcu/assert.h"
#include "halmcu/compiler.h"

void halmcu_assertion_failed(const uintptr_t *pc, const uintptr_t *lr) {
	unused(pc);
	unused(lr);
	mock().actualCall(__func__);
}
