#include "abov/irq.h"
#include "abov/asm/arm/cmsis.h"
#include "abov/compiler.h"

ABOV_STATIC_ASSERT(IRQ_FIXED == 16, "");

void irq_enable(irq_t irq)
{
	unsigned int n = (unsigned int)irq;
	if (n < IRQ_FIXED || n >= IRQ_MAX) {
		return;
	}
	n -= IRQ_FIXED;

	NVIC->ISER[n >> 5] = 1UL << (n & 0x1f);
}

void irq_disable(irq_t irq)
{
	unsigned int n = (unsigned int)irq;
	if (n < IRQ_FIXED || n >= IRQ_MAX) {
		return;
	}
	n -= IRQ_FIXED;

	NVIC->ICER[n >> 5] = 1UL << (n & 0x1f);
	__DSB();
	__ISB();
}

void irq_set_priority(irq_t irq, int priority)
{
	int n = (int)irq - IRQ_FIXED;
	__NVIC_SetPriority((IRQn_Type)n, (uint32_t)priority);
}
