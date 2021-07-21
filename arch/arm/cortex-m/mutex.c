#include "halmcu/mutex.h"
#include "halmcu/asm/arm/atomic.h"
#include "halmcu/compiler.h"

HALMCU_WEAK
void halmcu_mutex_lock(halmcu_mutex_t *l)
{
	uintptr_t val;

	do {
		val = atomic_ll(l);
		if (val != HALMCU_MUTEX_LOCKED &&
				atomic_sc(HALMCU_MUTEX_LOCKED, l) == 0) {
			break;
		}
	} while (val == HALMCU_MUTEX_LOCKED);
}

HALMCU_WEAK
void halmcu_mutex_unlock(halmcu_mutex_t *l)
{
	uintptr_t val;

	do {
		val = atomic_ll(l);
		if (val == HALMCU_MUTEX_UNLOCKED) {
			break;
		}
	} while (atomic_sc(HALMCU_MUTEX_UNLOCKED, l) != 0);
}

HALMCU_WEAK
void halmcu_mutex_init(halmcu_mutex_t *l)
{
	*l = HALMCU_MUTEX_UNLOCKED;
}
