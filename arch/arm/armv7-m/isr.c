#include "nvic.h"
#include <stdint.h>
#include <assert.h>
#include "abov/compiler.h"

extern uintptr_t _estack;

ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_nmi(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_hardfault(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_memmanage(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_busfault(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_usagefault(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_svc(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_debugmonitor(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_pendsv(void);
ABOV_WEAK ABOV_ALIAS("default_fallback_handler") void ISR_systick(void);

ABOV_WEAK
ABOV_USED
ABOV_SECTION(".isr_vectors")
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
};

ABOV_WEAK ABOV_USED void default_fallback_handler(void)
{
	assert(0);
}
