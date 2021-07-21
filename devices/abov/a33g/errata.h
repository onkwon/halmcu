#ifndef HALMCU_ERRATA_H
#define HALMCU_ERRATA_H

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * @todo TODO: PMULEGACY->LEGACY should be set before running the device
 * according to the official example code of A33G52x. don't know what and why
 * it does so?
 */
void errata_1(void);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_ERRATA_H */
