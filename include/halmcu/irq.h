#ifndef HALMCU_IRQ_H
#define HALMCU_IRQ_H

#if defined(__cplusplus)
extern "C" {
#endif

#define PERIPH_TO_IRQ(periph)		((irq_t)((periph) + IRQ_FIXED))

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
 * @brief Enable an interrupt
 *
 * @param[in] irq a enum of @ref irq_t
 */
void irq_enable(irq_t irq);
/**
 * @brief Disable an interrupt
 *
 * @param[in] irq a enum of @ref irq_t
 */
void irq_disable(irq_t irq);
/**
 * @brief Clear pending bit on the given interrupt
 *
 * @param[in] irq a enum of @ref irq_t
 */
void irq_clear_pending(irq_t irq);
/**
 * @brief Set the priority for an interrupt
 *
 * @param[in] irq a enum of @ref irq_t
 * @param[in] priority supports 0 to 192. The smaller number the higher priority
 */
void irq_set_priority(irq_t irq, int priority);
/**
 * @brief Generate software interrupt
 *
 * @param[in] irq a enum of @ref irq_t
 */
void irq_raise(irq_t irq);

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

#endif /* HALMCU_IRQ_H */
