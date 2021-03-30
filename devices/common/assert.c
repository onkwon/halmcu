#include <assert.h>
#include "abov/compiler.h"

ABOV_WEAK
ABOV_NORETURN
void __assert_func(const char * const file,
		const int line, const char * const func, const char * const exp)
{
	unused(file);
	unused(line);
	unused(func);
	unused(exp);

	while (1) { /* hang */ }
}
