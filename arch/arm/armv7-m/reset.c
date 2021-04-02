#include "abov/irq.h"
#include <stdint.h>
#include "abov/asm/arm/cmsis.h"
#include "abov/compiler.h"
#include "abov/system.h"

extern uintptr_t _data, _edata, _ebss, _data_loadaddr;
extern int main(void);

static inline void initialize_core(void)
{
	SCB->CCR |= SCB_CCR_STKALIGN_Msk; /* 8-byte stack alignment */
	__ISB();
	__DSB();
}

static inline void initialize_ram(void)
{
	volatile const uintptr_t *src = &_data_loadaddr;
	uintptr_t *dst = &_data;

	while (dst < &_edata) {
		*dst++ = *src++;
	}

	while (dst < &_ebss) {
		*dst++ = 0;
	}

	mb();
}

static inline void run_constructors(void)
{
}

static inline void run_destructors(void)
{
}

ABOV_WEAK void early_init(void)
{
}

ABOV_WEAK void ISR_reset(void)
{
	initialize_core();
	initialize_ram();

	early_init();

	run_constructors();
	main();
	run_destructors();
}
