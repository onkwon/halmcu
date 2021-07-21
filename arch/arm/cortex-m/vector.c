#include "halmcu/irq.h"
#include <stdint.h>
#include "halmcu/compiler.h"
#include "halmcu/assert.h"

typedef void (*nvic_vector_t)(void);

extern uintptr_t _estack;

HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_nmi(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_hardfault(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_memmanage(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_busfault(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_usagefault(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_svc(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_debugmonitor(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_pendsv(void);
HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_systick(void);

#define DEFINE_IRQ(n, name) \
	HALMCU_WEAK HALMCU_ALIAS("irq_default_handler") void ISR_ ## name(void);
#define RESERVE_IRQ(n)
#include IRQ_DEFINES
#undef RESERVE_IRQ
#undef DEFINE_IRQ

HALMCU_WEAK
HALMCU_USED
HALMCU_SECTION(".isr_vectors")
const nvic_vector_t isr_vectors[] = {
	(nvic_vector_t)&_estack,
	ISR_reset,
	ISR_nmi,
	ISR_hardfault,
	ISR_memmanage,
	ISR_busfault,
	ISR_usagefault,
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	ISR_svc,
	ISR_debugmonitor,
	0, /* reserved */
	ISR_pendsv,
	ISR_systick,
#define DEFINE_IRQ(n, name) ISR_ ## name,
#define RESERVE_IRQ(n) 0,
#include IRQ_DEFINES
#undef RESERVE_IRQ
#undef DEFINE_IRQ
};

HALMCU_WEAK HALMCU_USED void irq_default_handler(void)
{
	assert(0);
}
