#ifndef ABOV_SYSTEM_H
#define ABOV_SYSTEM_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize system in early booting time.
 *
 * Early low-level initialization can be done depending on vendor specific such
 * as co-processor configuration, initializing external memory, enabling caches
 * and so on.
 */
void system_early_init(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SYSTEM_H */
