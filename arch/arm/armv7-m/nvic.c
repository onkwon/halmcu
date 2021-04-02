#include "abov/asm/arm/nvic.h"
#include "abov/asm/arm/cmsis.h"

void nvic_enable_irq(uint32_t nirq)
{
	NVIC->ISER[nirq >> 5] = 1UL << (nirq & 0x1f);
}

void nvic_disable_irq(uint32_t nirq)
{
	NVIC->ICER[nirq >> 5] = 1UL << (nirq & 0x1f);
	__DSB();
	__ISB();
}
