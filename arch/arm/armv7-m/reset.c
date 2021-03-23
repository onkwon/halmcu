#include "nvic.h"
#include <stdint.h>
#include "cmsis.h"
#include "abov/compiler.h"
#include "abov/system.h"

extern uintptr_t _data, _edata, _ebss, _data_loadaddr;
extern int main(void);

static inline void initialize_core(void)
{
	SCB->CCR |= SCB_CCR_STKALIGN_Msk; /* 8-byte stack alignment */
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
}

static inline void run_constructors(void)
{
}

static inline void run_destructors(void)
{
}

ABOVE_WEAK void system_early_init(void)
{
}

ABOVE_WEAK void ISR_reset(void)
{
	initialize_core();
	initialize_ram();

	system_early_init();

	run_constructors();

	main();

	run_destructors();
}
