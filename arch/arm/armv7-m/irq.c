#include "abov/irq.h"
#include "abov/asm/arm/cmsis.h"

void irq_enable(irq_t irq)
{
	NVIC->ISER[irq >> 5] = 1UL << (irq & 0x1f);
}

void irq_disable(irq_t irq)
{
	NVIC->ICER[irq >> 5] = 1UL << (irq & 0x1f);
	__DSB();
	__ISB();
}
