#ifndef HALMCU_MUTEX_H
#define HALMCU_MUTEX_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#if !defined(HALMCU_MUTEX_LOCKED)
#define HALMCU_MUTEX_LOCKED			0
#endif
#if !defined(HALMCU_MUTEX_UNLOCKED)
#define HALMCU_MUTEX_UNLOCKED			1
#endif

#define HALMCU_DEFINE_MUTEX(name) \
	halmcu_mutex_t name = HALMCU_MUTEX_UNLOCKED

typedef uintptr_t halmcu_mutex_t;

void halmcu_mutex_init(halmcu_mutex_t *l);
void halmcu_mutex_lock(halmcu_mutex_t *l);
void halmcu_mutex_unlock(halmcu_mutex_t *l);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_MUTEX_H */
