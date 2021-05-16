#include "abov/assert.h"
#include "abov/compiler.h"

ABOV_WEAK
ABOV_NORETURN
void abov_assertion_failed(const uintptr_t *pc, const uintptr_t *lr)
{
	unused(pc);
	unused(lr);
	abov_raise_trap(0);
	while (1) { /* hang */ }
	__builtin_unreachable();
}
