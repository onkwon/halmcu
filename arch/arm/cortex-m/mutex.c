#include "abov/mutex.h"
#include "abov/asm/arm/atomic.h"
#include "abov/compiler.h"

ABOV_WEAK
void abov_mutex_lock(abov_mutex_t *l)
{
	uintptr_t val;

	do {
		val = atomic_ll(l);
		if (val != ABOV_MUTEX_LOCKED &&
				atomic_sc(ABOV_MUTEX_LOCKED, l) == 0) {
			break;
		}
	} while (val == ABOV_MUTEX_LOCKED);
}

ABOV_WEAK
void abov_mutex_unlock(abov_mutex_t *l)
{
	uintptr_t val;

	do {
		val = atomic_ll(l);
		if (val == ABOV_MUTEX_UNLOCKED) {
			break;
		}
	} while (atomic_sc(ABOV_MUTEX_UNLOCKED, l) != 0);
}

ABOV_WEAK
void abov_mutex_init(abov_mutex_t *l)
{
	*l = ABOV_MUTEX_UNLOCKED;
}
