#include "halmcu/assert.h"
#include "halmcu/compiler.h"

HALMCU_WEAK
HALMCU_NORETURN
void halmcu_assertion_failed(const uintptr_t *pc, const uintptr_t *lr)
{
	unused(pc);
	unused(lr);
	halmcu_raise_trap(0);
	while (1) { /* hang */ }
	__builtin_unreachable();
}
