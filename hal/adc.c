#include "abov/hal/adc.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

void adc_enable(periph_t adc)
{
	pwr_enable_peripheral(adc);
	clk_ll_enable_peripheral(adc);

	adc_ll_enable(adc);
}

void adc_disable(periph_t adc)
{
	adc_ll_disable(adc);

	clk_ll_disable_peripheral(adc);
	pwr_disable_peripheral(adc);
}

void adc_set_mode(periph_t adc, adc_mode_t mode)
{
	adc_ll_set_mode(adc, mode);
}

void adc_start(periph_t adc)
{
	adc_ll_start(adc);
}

void adc_stop(periph_t adc)
{
	adc_ll_stop(adc);
}

void adc_select_channel(periph_t adc, adc_channel_t channel)
{
	adc_ll_select_channel(adc, channel);
}

void adc_set_trigger(periph_t adc, adc_trigger_t trigger)
{
	adc_ll_set_trigger(adc, trigger);
}

uint32_t adc_get_measurement(periph_t adc)
{
	return adc_ll_get_measurement(adc);
}

void adc_enable_irq(periph_t adc)
{
	adc_ll_enable_irq(adc);
}

void adc_disable_irq(periph_t adc)
{
	adc_ll_disable_irq(adc);
}

adc_event_t adc_get_event(periph_t adc)
{
	return adc_ll_get_event(adc);
}

void adc_clear_event(periph_t adc, adc_event_t events)
{
	adc_ll_clear_event(adc, events);
}

bool adc_is_busy(periph_t adc)
{
	return adc_ll_is_busy(adc);
}

bool adc_is_completed(periph_t adc)
{
	return adc_ll_is_completed(adc);
}

void adc_set_sample_time(periph_t adc, adc_channel_t channel, uint32_t cycle)
{
	adc_ll_set_sample_time(adc, channel, cycle);
}

void adc_calibrate(periph_t adc)
{
	adc_ll_calibrate(adc);
}
