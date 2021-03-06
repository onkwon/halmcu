#include "CppUTestExt/MockSupport.h"

#include "halmcu/irq.h"

void irq_enable(irq_t irq)
{
	mock().actualCall(__func__).withParameter("irq", irq);
}

void irq_disable(irq_t irq)
{
	mock().actualCall(__func__).withParameter("irq", irq);
}

void irq_set_priority(irq_t irq, int priority)
{
	mock().actualCall(__func__)
		.withParameter("irq", irq)
		.withParameter("priority", priority);
}
