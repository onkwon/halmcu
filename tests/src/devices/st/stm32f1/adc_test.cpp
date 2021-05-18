#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/adc.h"
#include "stm32f1.h"
#include "abov/assert.h"

extern "C" {
static ADC_Type adcreg1, adcreg2, adcreg3;
static RCC_Type rcc_reg;
ADC_Type * const ADC1 = &adcreg1;
ADC_Type * const ADC2 = &adcreg2;
ADC_Type * const ADC3 = &adcreg3;
RCC_Type * const RCC = &rcc_reg;
}

TEST_GROUP(adc) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(ADC1, 0, sizeof(*ADC1));
		memset(ADC2, 0, sizeof(*ADC2));
		memset(ADC3, 0, sizeof(*ADC3));
		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(adc, is_completed_ShouldReturnTrue_WhenCoversionEnd) {
	ADC1->SR = 2;
	LONGS_EQUAL(true, adc_ll_is_completed(PERIPH_ADC1));
}

TEST(adc, enable_ShouldSetADON) {
	adc_ll_enable(PERIPH_ADC1);
	LONGS_EQUAL(1, ADC1->CR2);
}

TEST(adc, disable_ShouldClearADON) {
	ADC1->CR2 = 1U;
	adc_ll_disable(PERIPH_ADC1);
	LONGS_EQUAL(0, ADC1->CR2);
}

TEST(adc, set_mode_ShouldSetCONT_WhenContinuousConversionModeGiven) {
	adc_ll_set_mode(PERIPH_ADC2, ADC_MODE_CONTINUOUS_CONVERSION);
	LONGS_EQUAL(2, ADC2->CR2);
	adc_ll_set_mode(PERIPH_ADC3, ADC_MODE_CONTINUOUS_CONVERSION_MULTI_CHANNEL);
	LONGS_EQUAL(2, ADC3->CR2);
}
TEST(adc, set_mode_ShouldClearCONT_WhenSingleConversionModeGiven) {
	ADC1->CR2 = 2;
	adc_ll_set_mode(PERIPH_ADC1, ADC_MODE_SINGLE_CONVERSION);
	LONGS_EQUAL(0, ADC1->CR2);
}

TEST(adc, set_trigger_ShouldSetExtSel) {
	adc_ll_set_trigger(PERIPH_ADC1, ADC_TRIGGER_MANUAL);
	LONGS_EQUAL(7, ADC1->CR2 >> 17);
	adc_ll_set_trigger(PERIPH_ADC1, ADC_TRIGGER_TIMER1_CC1);
	LONGS_EQUAL(0, ADC1->CR2 >> 17);
	adc_ll_set_trigger(PERIPH_ADC1, ADC_TRIGGER_EXTI11);
	LONGS_EQUAL(6, ADC1->CR2 >> 17);
	adc_ll_set_trigger(PERIPH_ADC3, ADC_TRIGGER_MANUAL);
	LONGS_EQUAL(7, ADC3->CR2 >> 17);
	adc_ll_set_trigger(PERIPH_ADC3, ADC_TRIGGER_TIMER3_CC1);
	LONGS_EQUAL(0, ADC3->CR2 >> 17);
}

TEST(adc, start_ShouldStartConversion) {
	adc_ll_start(PERIPH_ADC1);
	LONGS_EQUAL(0x500000, ADC1->CR2);
}
TEST(adc, stop_ShouldStopConversion) {
	ADC1->CR2 = 2;
	adc_ll_stop(PERIPH_ADC1);
	LONGS_EQUAL(0, ADC1->CR2);
}

TEST(adc, select_channel_ShouldSetSequenceLength) {
	adc_ll_select_channel(PERIPH_ADC1, ADC_CHANNEL_0);
	LONGS_EQUAL(1, ADC1->SQR1 >> 20);
	adc_ll_select_channel(PERIPH_ADC1, (adc_channel_t)
			(ADC_CHANNEL_0 | ADC_CHANNEL_1));
	LONGS_EQUAL(2, ADC1->SQR1 >> 20);
	adc_ll_select_channel(PERIPH_ADC1, (adc_channel_t)
			(ADC_CHANNEL_0 | ADC_CHANNEL_1 | ADC_CHANNEL_15));
	LONGS_EQUAL(3, ADC1->SQR1 >> 20);
}
TEST(adc, select_channel_ShouldSetSequence) {
	adc_ll_select_channel(PERIPH_ADC1, ADC_CHANNEL_1);
	LONGS_EQUAL(1, ADC1->SQR3);
	adc_ll_select_channel(PERIPH_ADC1, (adc_channel_t)
			(ADC_CHANNEL_0 | ADC_CHANNEL_1));
	LONGS_EQUAL(0x20, ADC1->SQR3);
	adc_ll_select_channel(PERIPH_ADC1, (adc_channel_t)
			(ADC_CHANNEL_0 | ADC_CHANNEL_1 | ADC_CHANNEL_12));
	LONGS_EQUAL(0x3020, ADC1->SQR3);
}

TEST(adc, get_measument_ShouldReturnDR) {
	ADC1->DR = 0xa55a;
	LONGS_EQUAL(0xa55a, adc_ll_get_measurement(PERIPH_ADC1));
}

TEST(adc, set_sample_time_ShouldSetSMPR) {
	adc_ll_set_sample_time(PERIPH_ADC1, ADC_CHANNEL_0, 200);
	LONGS_EQUAL(7, ADC1->SMPR2);
	adc_ll_set_sample_time(PERIPH_ADC1, ADC_CHANNEL_9, 50);
	LONGS_EQUAL(0x28000007, ADC1->SMPR2);
	adc_ll_set_sample_time(PERIPH_ADC1, ADC_CHANNEL_13, 20);
	LONGS_EQUAL(0x600, ADC1->SMPR1);
}

TEST(adc, enable_irq_ShouldSetEOC) {
	adc_ll_enable_irq(PERIPH_ADC1);
	LONGS_EQUAL(0x20, ADC1->CR1);
}

TEST(adc, disable_irq_ShouldClearEOC) {
	ADC1->CR1 = 0x20;
	adc_ll_disable_irq(PERIPH_ADC1);
	LONGS_EQUAL(0, ADC1->CR1);
}

TEST(adc, get_event_ShouldReturnEvent) {
	LONGS_EQUAL(ADC_EVENT_NONE, adc_ll_get_event(PERIPH_ADC1));
	ADC1->SR = 2;
	LONGS_EQUAL(ADC_EVENT_COMPLETE, adc_ll_get_event(PERIPH_ADC1));
}

TEST(adc, clear_event_ShouldClearEvent) {
	ADC1->SR = 2;
	adc_ll_clear_event(PERIPH_ADC1, ADC_EVENT_COMPLETE);
	LONGS_EQUAL(0, ADC1->SR);
}
