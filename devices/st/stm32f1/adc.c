#include "abov/ll/adc.h"

#include <stddef.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/assert.h"
#include "stm32f1.h"

static ADC_Type *get_interface_from_periph(periph_t adc)
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

void adc_reset(periph_t adc)
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

bool adc_is_completed(periph_t adc)
{
	ADC_Type *reg = get_interface_from_periph(adc);
	return !!(reg->SR & 2); /* EOC */
}

void adc_activate(periph_t adc)
{
	ADC_Type *reg = get_interface_from_periph(adc);
	bitop_set(&reg->CR2, 0/*ADON*/);
	/* NOTE: tSTAB delay needed between power up and start of conversion */
}

void adc_deactivate(periph_t adc)
{
	ADC_Type *reg = get_interface_from_periph(adc);
	bitop_clear(&reg->CR2, 0/*ADON*/);
}

/* NOTE: calibration recommended after each power up */
/* NOTE: at least two clock cycle should be given after power-up before
 * starting calibration. */
void adc_calibrate(periph_t adc)
{
	ADC_Type *reg = get_interface_from_periph(adc);
	bitop_set(&reg->CR2, 2/*CAL*/);
	while ((reg->CR2 & (1U << 2))) {
		/* waiting */
	}
	/* calibration code stored in DR register */
}
