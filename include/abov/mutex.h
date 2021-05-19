#ifndef ABOV_MUTEX_H
#define ABOV_MUTEX_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#if !defined(ABOV_MUTEX_LOCKED)
#define ABOV_MUTEX_LOCKED			0
#endif
#if !defined(ABOV_MUTEX_UNLOCKED)
#define ABOV_MUTEX_UNLOCKED			1
#endif

#define ABOV_DEFINE_MUTEX(name) \
	abov_mutex_t name = ABOV_MUTEX_UNLOCKED

typedef uintptr_t abov_mutex_t;

void abov_mutex_init(abov_mutex_t *l);
void abov_mutex_lock(abov_mutex_t *l);
void abov_mutex_unlock(abov_mutex_t *l);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_MUTEX_H */
