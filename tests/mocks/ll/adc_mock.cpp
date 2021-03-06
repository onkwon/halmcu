#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/adc.h"

void adc_reset(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_enable_clock(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_disable_clock(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("hz", hz)
		.withParameter("pclk", pclk);
}
void adc_set_mode(periph_t adc, adc_mode_t mode) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("mode", mode);
}
void adc_start(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_stop(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_select_channel(periph_t adc, adc_channel_t channel) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("channel", channel);
}
void adc_set_trigger(periph_t adc, adc_trigger_t trigger) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("trigger", trigger);
}
uint32_t adc_get_measurement(periph_t adc) {
	return mock().actualCall(__func__)
		.withParameter("adc", adc)
		.returnUnsignedIntValueOrDefault(0);
}
void adc_enable_irq(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_disable_irq(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
adc_event_t adc_get_event(periph_t adc) {
	return (adc_event_t)mock().actualCall(__func__)
		.withParameter("adc", adc)
		.returnUnsignedIntValueOrDefault(ADC_EVENT_NONE);
}
void adc_clear_event(periph_t adc, adc_event_t events) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("events", events);
}
bool adc_is_busy(periph_t adc) {
	return mock().actualCall(__func__)
		.withParameter("adc", adc)
		.returnBoolValueOrDefault(true);
}
bool adc_is_completed(periph_t adc) {
	return mock().actualCall(__func__)
		.withParameter("adc", adc)
		.returnBoolValueOrDefault(false);
}
void adc_set_sample_time(periph_t adc, adc_channel_t channel, uint32_t cycle) {
	mock().actualCall(__func__)
		.withParameter("adc", adc)
		.withParameter("channel", channel)
		.withParameter("cycle", cycle);
}
void adc_calibrate(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
