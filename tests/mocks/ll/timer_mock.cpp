#include "CppUTestExt/MockSupport.h"

#include "abov/ll/timer.h"

void timer_ll_reset(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void timer_ll_set_mode(periph_t peri, timer_mode_t mode)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("mode", mode);
}

uint32_t timer_ll_get_frequency(periph_t peri, uint32_t tclk)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("tclk", tclk)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_ll_enable_irq(periph_t peri, timer_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("events", events);
}

void timer_ll_start(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void timer_ll_stop(periph_t peri)
{
	mock().actualCall(__func__).withParameter("peri", peri);
}

void timer_ll_set_prescaler(periph_t peri, uint32_t div_factor)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("div_factor", div_factor);
}

uint32_t timer_ll_get_prescaler(periph_t peri)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_ll_set_reload(periph_t peri, uint32_t value)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("value", value);
}

uint32_t timer_ll_get_reload(periph_t peri)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_ll_set_cc_pin_polarity(periph_t peri, timer_cc_t cc, bool active_high)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc)
		.withParameter("active_high", active_high);
}

void timer_ll_set_clock_divider(periph_t peri, uint32_t div_factor)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("div_factor", div_factor);
}

void timer_ll_set_cc(periph_t peri, timer_cc_t cc, uint32_t value)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc)
		.withParameter("value", value);
}

uint32_t timer_ll_get_cc(periph_t peri, timer_cc_t cc)
{
	return mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_ll_clear_event(periph_t peri, timer_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("events", events);
}

timer_event_t timer_ll_get_event(periph_t peri)
{
	return (timer_event_t)mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(0);
}

void timer_ll_enable_cc_pin(periph_t peri, timer_cc_t cc)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc);
}

void timer_ll_disable_cc_pin(periph_t peri, timer_cc_t cc)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc);
}

void timer_ll_enable_cc_preload(periph_t peri, timer_cc_t cc)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc);
}

void timer_ll_disable_cc_preload(periph_t peri, timer_cc_t cc)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc);
}

void timer_ll_set_cc_pin_mode(periph_t peri, timer_cc_t cc, timer_cc_mode_t mode)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc)
		.withParameter("mode", mode);
}

void timer_ll_set_cc_pin(periph_t peri, timer_cc_t cc, uint32_t value)
{
	mock().actualCall(__func__)
		.withParameter("peri", peri)
		.withParameter("cc", cc)
		.withParameter("value", value);
}
