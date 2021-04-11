#include "CppUTestExt/MockSupport.h"

#include "abov/irq.h"

void irq_enable(irq_t irq)
{
	mock().actualCall(__func__).withParameter("irq", irq);
}

void irq_disable(irq_t irq)
{
	mock().actualCall(__func__).withParameter("irq", irq);
}
