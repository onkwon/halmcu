#ifndef ABOV_SYSTEM_H
#define ABOV_SYSTEM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h> /* needed by cmsis_gcc.h */
#include "cmsis_gcc.h"
#include "abov/compiler.h"

#include "abov/errata.h"

/**
 * Initialize system in early booting time.
 *
 * Early low-level initialization can be done depending on vendor specifics such
 * as co-processor configuration, initializing external memory, enabling caches
 * and so on.
 */
void early_init(void);

/** Insert a hardware full memory bariier. */
#define mb()					__DMB()
/** Insert a hardware read access memory barrier. */
#define rmb()					barrier()
/** Insert a hardware write access memory barrier. */
#define wmb()					barrier()

/** Enable IRQ Interrupts */
#define interrupt_unlock()			__enable_irq()
/** Disable IRQ Interrupts */
#define interrupt_lock()			__disable_irq()

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SYSTEM_H */
