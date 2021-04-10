#ifndef ABOV_IRQ_H
#define ABOV_IRQ_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PERI_TO_IRQ(peri)		((irq_t)((peri) + IRQ_FIXED))

#define DEFINE_IRQ(n, name) IRQ_ ## name = (n) + IRQ_FIXED,
#define RESERVE_IRQ(n)
typedef enum {
#if !defined(IRQ_FIXED)
	IRQ_FIXED			= 16,
#endif
#include IRQ_DEFINES
	IRQ_MAX,
	IRQ_UNDEFINED			= IRQ_MAX,
} irq_t;
#undef RESERVE_IRQ
#undef DEFINE_IRQ

/**
 * Enable an interrupt
 *
 * :param irq: a enum of :c:type:`irq_t`
 */
void irq_enable(irq_t irq);
/**
 * Disable an interrupt
 *
 * :param irq: a enum of :c:type:`irq_t`
 */
void irq_disable(irq_t irq);
/**
 * Set the priority for an interrupt
 *
 * :param irq: a enum of :c:type:`irq_t`
 * :param priority: supports 0 to 192. The smaller number the higher priority
 */
void irq_set_priority(irq_t irq, int priority);

void irq_default_handler(void);

void ISR_reset(void);
void ISR_nmi(void);
void ISR_hardfault(void);
void ISR_memmanage(void);
void ISR_busfault(void);
void ISR_usagefault(void);
void ISR_svc(void);
void ISR_debugmonitor(void);
void ISR_pendsv(void);
void ISR_systick(void);

#define DEFINE_IRQ(n, name) void ISR_ ## name(void);
#define RESERVE_IRQ(n)
#include IRQ_DEFINES
#undef RESERVE_IRQ
#undef DEFINE_IRQ

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_IRQ_H */
