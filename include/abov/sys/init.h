#ifndef ABOV_INIT_H
#define ABOV_INIT_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * Initialize system in early booting time.
 *
 * Early low-level initialization can be done depending on vendor specifics such
 * as co-processor configuration, initializing external memory, enabling caches
 * and so on.
 */
void sys_early_init(void);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_INIT_H */
