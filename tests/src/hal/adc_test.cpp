#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/adc.h"

void adc_activate(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}
void adc_deactivate(periph_t adc) {
	mock().actualCall(__func__).withParameter("adc", adc);
}

TEST_GROUP(ADC) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(ADC, enable_ShouldEnablePeripheralAndClock) {
	mock().expectOneCall("pwr_enable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("clk_enable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("adc_activate")
		.withParameter("adc", PERIPH_ADC1);
	adc_enable(PERIPH_ADC1);
}

TEST(ADC, disable_ShouldDisablePeripheralAndClock) {
	mock().expectOneCall("pwr_disable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("clk_disable_peripheral")
		.withParameter("peri", PERIPH_ADC1);
	mock().expectOneCall("adc_deactivate")
		.withParameter("adc", PERIPH_ADC1);
	adc_disable(PERIPH_ADC1);
}
