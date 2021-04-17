#include "abov/ll/adc.h"

#include <assert.h>

#include "abov/compiler.h"
#include "abov/bitop.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define ADIF				(1U << 5)

void adc_reset(peripheral_t adc)
{
	unused(adc);

	ADC->CR |= ADIF;
	ADC->CR = 0;
	ADC->MR = 0x40ff;
	ADC->TEST = 0;
}

void adc_enable(peripheral_t adc)
{
	unused(adc);
	bitop_set(&ADC->MR, 15);
}

void adc_disable(peripheral_t adc)
{
	unused(adc);
	bitop_clear(&ADC->MR, 15);
}

void adc_set_mode(peripheral_t adc, adc_mode_t mode)
{
	unused(adc);

	switch (mode) {
	case ADC_MODE_NORMAL:
		bitop_clear(&ADC->MR, 14); /* ADSTBY */
		break;
	case ADC_MODE_IDLE:
		bitop_set(&ADC->MR, 14);
		break;
	default:
		break;
	}
}

void adc_start(peripheral_t adc)
{
	unused(adc);
	bitop_clear(&ADC->CR, 8); /* ADEOC */
	bitop_set(&ADC->CR, 7); /* ADST */
}

void adc_stop(peripheral_t adc)
{
	unused(adc);
	bitop_clear(&ADC->CR, 7); /* ADST */
}

void adc_select_channel(peripheral_t adc, adc_channel_t channel)
{
	unused(adc);
	if (channel > ADC_CHANNEL_15) {
		return;
	}

	bitop_clean_set_with_mask(&ADC->CR, 0, 0xf, channel);
}

void adc_set_trigger(peripheral_t adc, adc_trigger_t trigger)
{
	unused(adc);
	if (trigger > ADC_TRIGGER_TIMER7_CC_0) {
		return;
	}

	if (trigger == ADC_TRIGGER_MANUAL) {
		bitop_clear(&ADC->MR, 11); /* EXTRG */
	} else {
		bitop_clean_set_with_mask(&ADC->MR, 8, 7U, trigger - 1);
		bitop_set(&ADC->MR, 11); /* EXTRG */
	}
}

void adc_enable_irq(peripheral_t adc)
{
	unused(adc);
	bitop_set(&ADC->MR, 12); /* ADIE */
}

void adc_disable_irq(peripheral_t adc)
{
	unused(adc);
	bitop_clear(&ADC->MR, 12); /* ADIE */
}

uint32_t adc_read(peripheral_t adc)
{
	unused(adc);
	return ADC->DR >> 3;
}

bool adc_is_busy(peripheral_t adc)
{
	unused(adc);
	return !(ADC->CR & 0x40); /* AFLAG */
}

bool adc_is_completed(peripheral_t adc)
{
	unused(adc);
	return !!(ADC->CR & 0x100); /* ADEOC */
}

adc_event_t adc_get_event(peripheral_t adc)
{
	unused(adc);
	adc_event_t evt = ADC_EVENT_NONE;
	uint32_t cr = ADC->CR;

	if (cr & 0x100) { /* ADEOC */
		evt = (adc_event_t)(evt | ADC_EVENT_COMPLETE);
	}
	if (!(cr & 0x40)) { /* AFLAG */
		evt = (adc_event_t)(evt | ADC_EVENT_BUSY);
	}

	return evt;
}

void adc_clear_event(peripheral_t adc, adc_event_t events)
{
	unused(adc);
	unused(events);
	ADC->CR |= ADIF;
}

/*
 * | AVDD | Clock(MHz) | Conversion time(usec) |
 * | ---- | ---------- | --------------------- |
 * | 2.4  | 0.25       | 240                   |
 * | 2.7  | 1.00       | 60                    |
 * | 3.0  | 2.00       | 30                    |
 * | 4.0  | 3.00       | 20                    |
 * | 5.0  | 4.00       | 15                    |
 */
void adc_set_clock_frequency(peripheral_t adc, uint32_t hz, uint32_t pclk)
{
	unused(adc);
	assert(hz <= pclk && hz != 0);
	uint32_t cnt = pclk / hz - 1;
	bitop_clean_set_with_mask(&ADC->MR, 0, 0xff, cnt);
}
