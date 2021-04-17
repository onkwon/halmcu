#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/adc.h"
#include "a33g.h"
#include "abov/peripheral.h"

extern "C" {
static struct adc_stub adcreg;
struct adc_stub * const ADC = &adcreg;
}

TEST_GROUP(ADC) {
	void setup(void) {
		mock().ignoreOtherCalls();

		adc_reset(PERI_ADC);
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(ADC, reset_ShouldSetRegistersResetState) {
	LONGS_EQUAL(0, ADC->CR);
	LONGS_EQUAL(0x40ff, ADC->MR);
	LONGS_EQUAL(0, ADC->TEST);
}

TEST(ADC, enable_ShouldSetAdcen) {
	adc_enable(PERI_ADC);
	LONGS_EQUAL(0xc0ff, ADC->MR);
}

TEST(ADC, disable_ShouldClearAdcen) {
	adc_enable(PERI_ADC);
	adc_disable(PERI_ADC);
	LONGS_EQUAL(0x40ff, ADC->MR);
}

TEST(ADC, set_mode_ShouldSetADSTBY) {
	adc_set_mode(PERI_ADC, ADC_MODE_NORMAL);
	LONGS_EQUAL(0x00ff, ADC->MR);
	adc_set_mode(PERI_ADC, ADC_MODE_IDLE);
	LONGS_EQUAL(0x40ff, ADC->MR);
}

TEST(ADC, start_ShouldSetADSTAndClearAdeoc) {
	ADC->CR |= 0x100; /* ADEOC */
	adc_start(PERI_ADC);
	LONGS_EQUAL(0x80, ADC->CR);
}

TEST(ADC, stop_ShouldClearADST) {
	adc_start(PERI_ADC);
	adc_stop(PERI_ADC);
	LONGS_EQUAL(0, ADC->CR);
}

TEST(ADC, select_channel_ShouldSetADSEL) {
	adc_select_channel(PERI_ADC, ADC_CHANNEL_8);
	LONGS_EQUAL(8, ADC->CR);
	adc_select_channel(PERI_ADC, ADC_CHANNEL_3);
	LONGS_EQUAL(3, ADC->CR);
}
TEST(ADC, select_channel_ShouldDoNothing_WhenInvalidChannelGiven) {
	adc_select_channel(PERI_ADC, ADC_CHANNEL_MAX);
	LONGS_EQUAL(0, ADC->CR);
}

TEST(ADC, set_trigger_ShouldSetTselAndExtrg) {
	adc_set_trigger(PERI_ADC, ADC_TRIGGER_TIMER0_CC_0);
	LONGS_EQUAL(0x48ff, ADC->MR);
	adc_set_trigger(PERI_ADC, ADC_TRIGGER_MANUAL);
	LONGS_EQUAL(0x40ff, ADC->MR);
	adc_set_trigger(PERI_ADC, ADC_TRIGGER_TIMER1_CC_0);
	LONGS_EQUAL(0x49ff, ADC->MR);
	adc_set_trigger(PERI_ADC, ADC_TRIGGER_TIMER7_CC_0);
	LONGS_EQUAL(0x4fff, ADC->MR);
}

TEST(ADC, enable_irq_ShouldSetAdie) {
	adc_enable_irq(PERI_ADC);
	LONGS_EQUAL(0x50ff, ADC->MR);
}
TEST(ADC, disable_irq_ShouldClearAdie) {
	adc_enable_irq(PERI_ADC);
	adc_disable_irq(PERI_ADC);
	LONGS_EQUAL(0x40ff, ADC->MR);
}

TEST(ADC, is_busy_ShouldReturnTrue_WhenBusy) {
	LONGS_EQUAL(1, adc_is_busy(PERI_ADC));
}
TEST(ADC, is_busy_ShouldReturnFalse_WhenNotBusy) {
	ADC->CR |= 0x40;
	LONGS_EQUAL(0, adc_is_busy(PERI_ADC));
}
TEST(ADC, is_complete_ShouldReturnTrue_WhenConversionCompleted) {
	ADC->CR |= 0x100;
	LONGS_EQUAL(1, adc_is_completed(PERI_ADC));
}
TEST(ADC, is_complete_ShouldReturnFalse_WhenNotCompleted) {
	LONGS_EQUAL(0, adc_is_completed(PERI_ADC));
}

TEST(ADC, get_event_ShouldReturnComplete_WhenConversionDone) {
	ADC->CR = 0x140;
	LONGS_EQUAL(ADC_EVENT_COMPLETE, adc_get_event(PERI_ADC));
}

TEST(ADC, clear_event_ShouldSetAdif) {
	adc_clear_event(PERI_ADC, ADC_EVENT_COMPLETE);
	LONGS_EQUAL(0x20, ADC->CR);
}

TEST(ADC, set_clock_frequency_ShouldSetAdcs) {
	adc_set_clock_frequency(PERI_ADC, 4000000, 16000000);
	LONGS_EQUAL(3, ADC->MR & 0xff);
	adc_set_clock_frequency(PERI_ADC, 1000000, 1000000);
	LONGS_EQUAL(0, ADC->MR & 0xff);
	adc_set_clock_frequency(PERI_ADC, 250000, 1000000);
	LONGS_EQUAL(3, ADC->MR & 0xff);
}