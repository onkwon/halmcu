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
	LONGS_EQUAL(true, adc_is_completed(PERIPH_ADC1));
}

TEST(adc, activate_ShouldSetADON) {
	adc_activate(PERIPH_ADC1);
	LONGS_EQUAL(1, ADC1->CR2);
}

TEST(adc, deactivate_ShouldClearADON) {
	ADC1->CR2 = 1U;
	adc_deactivate(PERIPH_ADC1);
	LONGS_EQUAL(0, ADC1->CR2);
}
