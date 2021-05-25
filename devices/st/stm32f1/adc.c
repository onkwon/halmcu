#include "abov/ll/adc.h"

#include <stddef.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/assert.h"
#include "stm32f1.h"

static ADC_Type *get_instance(periph_t adc)
{
	switch (adc) {
	case PERIPH_ADC1:
		return ADC1;
	case PERIPH_ADC2:
		return ADC2;
	case PERIPH_ADC3:
		return ADC3;
	default:
		assert(0);
		return NULL;
	}
}

void adc_ll_reset(periph_t adc)
{
	uint32_t pos = 9;

	if (adc == PERIPH_ADC1) {
		pos = 9;
	} else if (adc == PERIPH_ADC2) {
		pos = 10;
	} else if (adc == PERIPH_ADC3) {
		pos = 15;
	} else {
		assert(0);
	}

	bitop_set(&RCC->APB2RSTR, pos);
	bitop_clear(&RCC->APB2RSTR, pos);
}

bool adc_ll_is_completed(periph_t adc)
{
	const ADC_Type *reg = get_instance(adc);
	return !!(reg->SR & 2); /* EOC */
}

void adc_ll_enable(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_set(&reg->CR2, 0/*ADON*/);
	/* NOTE: tSTAB delay needed between power up and start of conversion */
}

void adc_ll_disable(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_clear(&reg->CR2, 0/*ADON*/);
}

/* NOTE: calibration recommended after each power up */
/* NOTE: at least two clock cycle should be given after power-up before
 * starting calibration. */
void adc_ll_calibrate(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_set(&reg->CR2, 2/*CAL*/);
	while ((reg->CR2 & (1U << 2)) != 0) {
		/* waiting */
	}
	/* calibration code stored in DR register */
}

void adc_ll_set_mode(periph_t adc, adc_mode_t mode)
{
	ADC_Type *reg = get_instance(adc);
	bool continuous = false;

	if (mode == ADC_MODE_CONTINUOUS_CONVERSION
			|| mode == ADC_MODE_CONTINUOUS_CONVERSION_MULTI_CHANNEL) {
		continuous = true;
	}

	bitop_clean_set_with_mask(&reg->CR2, 1, 1, continuous);
}

static uint32_t get_adc1_2_trigger_value(adc_trigger_t trigger)
{
	switch (trigger) {
	case ADC_TRIGGER_MANUAL:
		return 7;
	case ADC_TRIGGER_TIMER1_CC1:
		return 0;
	case ADC_TRIGGER_TIMER1_CC2:
		return 1;
	case ADC_TRIGGER_TIMER1_CC3:
		return 2;
	case ADC_TRIGGER_TIMER2_CC2:
		return 3;
	case ADC_TRIGGER_TIMER3_TRGO:
		return 4;
	case ADC_TRIGGER_EXTI11:
		return 6;
	default:
		assert(0);
		return 0;
	}
}

static uint32_t get_adc3_trigger_value(adc_trigger_t trigger)
{
	switch (trigger) {
	case ADC_TRIGGER_MANUAL:
		return 7;
	case ADC_TRIGGER_TIMER3_CC1:
		return 0;
	default:
		assert(0);
		return 0;
	}
}

static uint32_t get_trigger_value(const ADC_Type *reg, adc_trigger_t trigger)
{
	if (reg == ADC3) {
		return get_adc3_trigger_value(trigger);
	}
	return get_adc1_2_trigger_value(trigger);
}

void adc_ll_set_trigger(periph_t adc, adc_trigger_t trigger)
{
	ADC_Type *reg = get_instance(adc);
	uint32_t val = get_trigger_value(reg, trigger);
	bitop_clean_set_with_mask(&reg->CR2, 17, 7, val);
}

void adc_ll_start(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_set(&reg->CR2, 20); /* EXTTRIG */
	bitop_set(&reg->CR2, 22); /* SWSTART */
}

void adc_ll_stop(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_clear(&reg->CR2, 1); /* CONT */
}

void adc_ll_select_channel(periph_t adc, adc_channel_t channel)
{
	ADC_Type *reg = get_instance(adc);
	uint32_t nch = 0;

	for (uint32_t i = 0; channel >> i; i++) {
		if (((channel >> i) & 1) == 0) {
			continue;
		}

		volatile uint32_t *sqr = &reg->SQR3;

		if ((nch / 6) == 1) {
			sqr = &reg->SQR2;
		} else if ((nch / 6) == 2) {
			sqr = &reg->SQR1;
		}

		bitop_clean_set_with_mask(sqr, nch * 5 % 30, 0x1f, i);

		nch++;
	}

	bitop_clean_set_with_mask(&reg->SQR1, 20, 0xf, nch);
}

bool adc_ll_is_busy(periph_t adc)
{
	const ADC_Type *reg = get_instance(adc);
	return reg->SR & 0x10; /* STRT */
}

uint32_t adc_ll_get_measurement(periph_t adc)
{
	const ADC_Type *reg = get_instance(adc);
	return reg->DR;
}

void adc_ll_set_sample_time(periph_t adc, adc_channel_t channel, uint32_t cycle)
{
	ADC_Type *reg = get_instance(adc);
	volatile uint32_t *smpr = &reg->SMPR2;
	uint32_t ch = (uint32_t)(__builtin_ffs((int)channel) - 1);
	uint32_t val = 0;

	if (channel > ADC_CHANNEL_9) {
		smpr = &reg->SMPR1;
	}

	if (cycle > 71) {
		val = 7;
	} else if (cycle > 55) {
		val = 6;
	} else if (cycle > 41) {
		val = 5;
	} else if (cycle > 28) {
		val = 4;
	} else if (cycle > 13) {
		val = 3;
	} else if (cycle > 7) {
		val = 2;
	} else if (cycle > 1) {
		val = 1;
	}

	bitop_clean_set_with_mask(smpr, ch * 3 % 30, 7, val);
}

void adc_ll_enable_irq(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_set(&reg->CR1, 5);
}

void adc_ll_disable_irq(periph_t adc)
{
	ADC_Type *reg = get_instance(adc);
	bitop_clear(&reg->CR1, 5);
}

adc_event_t adc_ll_get_event(periph_t adc)
{
	const ADC_Type *reg = get_instance(adc);
	uint32_t sr = reg->SR;

	if (sr & 2) {
		return ADC_EVENT_COMPLETE;
	}

	return ADC_EVENT_NONE;
}

void adc_ll_clear_event(periph_t adc, adc_event_t events)
{
	ADC_Type *reg = get_instance(adc);
	if (events & ADC_EVENT_COMPLETE) {
		bitop_clear(&reg->SR, 1);
	}
}

static uint32_t get_prescaler(void)
{
	return (RCC->CFGR >> 14/*ADCPRE*/) * 2 + 2;
}

static void set_prescaler(uint32_t val)
{
	bitop_clean_set_with_mask(&RCC->CFGR, 14/*ADCPRE*/, 3, val);
}

uint32_t adc_ll_get_frequency(periph_t adc, uint32_t pclk)
{
	unused(adc);
	return pclk / get_prescaler();
}

void adc_ll_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk)
{
	unused(adc);
	assert(hz <= 14000000); /* NOTE: must not exceed 14MHz on STM32F1 */
	for (uint32_t div = 2; div <= 8; div += 2) {
		if (pclk / div <= hz) {
			set_prescaler(div / 2 - 1);
			break;
		}
	}
}
