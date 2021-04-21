#include "CppUTestExt/MockSupport.h"

#include "abov/ll/timer.h"

void timer_reset(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void timer_set_mode(periph_t peri, timer_mode_t mode)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("mode", mode);
}

uint32_t timer_get_frequency(periph_t peri, uint32_t tclk)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("tclk", tclk)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_set_prescaler(periph_t peri, uint32_t div_factor)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("div_factor", div_factor);
}

void timer_enable_irq(periph_t peri, timer_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("events", events);
}
