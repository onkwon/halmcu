#include "halmcu/ll/adc.h"

#include "halmcu/assert.h"
#include "halmcu/compiler.h"
#include "halmcu/bitop.h"
#include "halmcu/asm/arm/cmsis.h"
#include "a33g.h"

#define ADIF				(1U << 5)

void adc_ll_reset(periph_t adc)
{
	unused(adc);

	ADC->CR |= ADIF;
	ADC->CR = 0;
	ADC->MR = 0x40ff;
	ADC->TEST = 0;
}

void adc_ll_enable(periph_t adc)
{
	unused(adc);
	bitop_set(&ADC->MR, 15);
}

void adc_ll_disable(periph_t adc)
{
	unused(adc);
	bitop_clear(&ADC->MR, 15);
}

void adc_ll_set_mode(periph_t adc, adc_mode_t mode)
{
	unused(adc);

	switch (mode) {
	case ADC_MODE_SINGLE_CONVERSION:
		bitop_clear(&ADC->MR, 14); /* ADSTBY */
		break;
	case ADC_MODE_IDLE:
		bitop_set(&ADC->MR, 14);
		break;
	default:
		assert(0);
		break;
	}
}

void adc_ll_start(periph_t adc)
{
	unused(adc);
	bitop_clear(&ADC->CR, 8); /* ADEOC */
	bitop_set(&ADC->CR, 7); /* ADST */
}

void adc_ll_stop(periph_t adc)
{
	unused(adc);
	bitop_clear(&ADC->CR, 7); /* ADST */
}

HALMCU_STATIC_ASSERT(__builtin_ffs(1) == 1, "");
void adc_ll_select_channel(periph_t adc, adc_channel_t channel)
{
	unused(adc);
	assert(channel & ((ADC_CHANNEL_15 << 1) - 1));
	bitop_clean_set_with_mask(&ADC->CR, 0, 0xf,
			(uint32_t)__builtin_ffs((int)channel) - 1);
}

void adc_ll_set_trigger(periph_t adc, adc_trigger_t trigger)
{
	unused(adc);
	assert(trigger <= ADC_TRIGGER_TIMER7_CC0);

	if (trigger == ADC_TRIGGER_MANUAL) {
		bitop_clear(&ADC->MR, 11); /* EXTRG */
	} else {
		bitop_clean_set_with_mask(&ADC->MR, 8, 7U, trigger - 1);
		bitop_set(&ADC->MR, 11); /* EXTRG */
	}
}

void adc_ll_enable_irq(periph_t adc)
{
	unused(adc);
	bitop_set(&ADC->MR, 12); /* ADIE */
}

void adc_ll_disable_irq(periph_t adc)
{
	unused(adc);
	bitop_clear(&ADC->MR, 12); /* ADIE */
}

uint32_t adc_ll_get_measurement(periph_t adc)
{
	unused(adc);
	return ADC->DR >> 4;
}

bool adc_ll_is_busy(periph_t adc)
{
	unused(adc);
	return !(ADC->CR & 0x40); /* AFLAG */
}

bool adc_ll_is_completed(periph_t adc)
{
	unused(adc);
	return !!(ADC->CR & 0x100); /* ADEOC */
}

adc_event_t adc_ll_get_event(periph_t adc)
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

void adc_ll_clear_event(periph_t adc, adc_event_t events)
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
void adc_ll_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk)
{
	unused(adc);
	assert(hz <= pclk && hz != 0);
	uint32_t cnt = pclk / hz - 1;
	bitop_clean_set_with_mask(&ADC->MR, 0, 0xff, cnt);
}
