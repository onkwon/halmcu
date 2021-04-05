#ifndef ABOV_SYSTEM_H
#define ABOV_SYSTEM_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h> /* needed by cmsis_gcc.h */
#include "cmsis_gcc.h"
#include "abov/compiler.h"

/** Early low-level initialization can be done depending on the core
 * implementation such as co-processor configuration, initializing external
 * memory, enabling caches and so on. */
void early_init(void);
/** Device specific initialization can be done before jumping into the main
 * application code. */
void pre_main(void);

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
