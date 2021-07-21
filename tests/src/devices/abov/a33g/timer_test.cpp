#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "halmcu/periph/timer.h"
#include "a33g.h"

extern "C" {
static uint8_t memspace[10 * 0x20];
uintptr_t T0_BASE = (uintptr_t)&memspace[0];
struct timer_stub * const T0 = (struct timer_stub *)&memspace[0];
struct timer_stub * const T1 = (struct timer_stub *)&memspace[0x20];
struct timer_stub * const T2 = (struct timer_stub *)&memspace[0x40];
struct timer_stub * const T3 = (struct timer_stub *)&memspace[0x60];
struct timer_stub * const T4 = (struct timer_stub *)&memspace[0x80];
struct timer_stub * const T5 = (struct timer_stub *)&memspace[0xA0];
struct timer_stub * const T6 = (struct timer_stub *)&memspace[0xC0];
struct timer_stub * const T7 = (struct timer_stub *)&memspace[0xE0];
struct timer_stub * const T8 = (struct timer_stub *)&memspace[0x100];
struct timer_stub * const T9 = (struct timer_stub *)&memspace[0x120];
}

TEST_GROUP(Timer) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(memspace, 0, sizeof(memspace));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Timer, set_prescaler_ShouldSetPrs) {
	timer_set_prescaler(PERIPH_TIMER0, 123);
	LONGS_EQUAL(123, T0->PRS);
	timer_set_prescaler(PERIPH_TIMER3, 2);
	LONGS_EQUAL(2, T3->PRS);
	timer_set_prescaler(PERIPH_TIMER9, 16);
	LONGS_EQUAL(16, T9->PRS);
}
TEST(Timer, get_prescaler_ShouldReturnPSC) {
	T0->PRS = 0xcafe;
	LONGS_EQUAL(0xcafe, timer_get_prescaler(PERIPH_TIMER0));
}

TEST(Timer, set_clock_divider_ShouldSetTimerClockSource) {
	timer_set_clock_divider(PERIPH_TIMER0, 4);
	LONGS_EQUAL(0x40, T0->CON);
	timer_set_clock_divider(PERIPH_TIMER0, 6);
	LONGS_EQUAL(0x60, T0->CON);
	timer_set_clock_divider(PERIPH_TIMER0, 1);
	LONGS_EQUAL(0x10, T0->CON);
	timer_set_clock_divider(PERIPH_TIMER0, 0);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, get_counter_ShouldReturnCntValue) {
	T0->CNT = 123;
	LONGS_EQUAL(123, timer_get_counter(PERIPH_TIMER0));
	T4->CNT = 1;
	LONGS_EQUAL(1, timer_get_counter(PERIPH_TIMER4));
	T8->CNT = 2;
	LONGS_EQUAL(2, timer_get_counter(PERIPH_TIMER8));
}

TEST(Timer, set_reload_ShouldSetGra) {
	timer_set_reload(PERIPH_TIMER0, 123);
	LONGS_EQUAL(123, T0->GRA);
	timer_set_reload(PERIPH_TIMER2, 3);
	LONGS_EQUAL(3, T2->GRA);
	timer_set_reload(PERIPH_TIMER5, 4);
	LONGS_EQUAL(4, T5->GRA);
}

TEST(Timer, get_reload_ShouldReturnGra) {
	T0->GRA = 123;
	LONGS_EQUAL(123, timer_get_reload(PERIPH_TIMER0));
	T6->GRA = 5;
	LONGS_EQUAL(5, timer_get_reload(PERIPH_TIMER6));
	T7->GRA = 6;
	LONGS_EQUAL(6, timer_get_reload(PERIPH_TIMER7));
}

TEST(Timer, set_mode_ShouldSetConMode) {
	timer_set_mode(PERIPH_TIMER0, TIMER_MODE_PWM);
	LONGS_EQUAL(1, T0->CON);
	timer_set_mode(PERIPH_TIMER0, TIMER_MODE_CAPTURE);
	LONGS_EQUAL(3, T0->CON);
	timer_set_mode(PERIPH_TIMER0, TIMER_MODE_ONESHOT);
	LONGS_EQUAL(2, T0->CON);
	timer_set_mode(PERIPH_TIMER0, TIMER_MODE_NORMAL);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, enable_irq_ShouldSetIrqFlags) {
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_OVERFLOW);
	LONGS_EQUAL(0x400, T0->CON);
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_CC_0);
	LONGS_EQUAL(0x500, T0->CON);
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_CC_1);
	LONGS_EQUAL(0x700, T0->CON);
}

TEST(Timer, enable_irq_ShouldDoNothing_WhenUnsupportedGiven) {
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_CC_2);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, disable_irq_ShouldClearIrqFlags) {
	T1->CON = 0x700;
	timer_disable_irq(PERIPH_TIMER1, TIMER_EVENT_OVERFLOW);
	LONGS_EQUAL(0x300, T1->CON);
	timer_disable_irq(PERIPH_TIMER1, TIMER_EVENT_CC_0);
	LONGS_EQUAL(0x200, T1->CON);
	timer_disable_irq(PERIPH_TIMER1, TIMER_EVENT_CC_1);
	LONGS_EQUAL(0, T1->CON);
}

TEST(Timer, clear_event_ShouldClearIrqFlag) {
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_OVERFLOW);
	LONGS_EQUAL(0x4000, T0->CON);
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_CC_0);
	LONGS_EQUAL(0x5000, T0->CON);
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_CC_1);
	LONGS_EQUAL(0x7000, T0->CON);
}

TEST(Timer, start_ShouldSetTEN) {
	timer_start(PERIPH_TIMER0);
	LONGS_EQUAL(3, T0->CMD); /* TEN + TCLR */
}

TEST(Timer, stop_ShouldClearTEN) {
	T0->CMD = 1;
	timer_stop(PERIPH_TIMER0);
	LONGS_EQUAL(0, T0->CMD);
}

TEST(Timer, set_cc_ShouldSetCompareRegister) {
	timer_set_cc(PERIPH_TIMER0, TIMER_CC_0, 1234);
	LONGS_EQUAL(1234, T0->GRA);
	timer_set_cc(PERIPH_TIMER1, TIMER_CC_1, 5678);
	LONGS_EQUAL(5678, T1->GRB);
}

TEST(Timer, get_cc_ShouldReturnCaptureRegister) {
	T0->GRA = 0xa5a5;
	T0->GRB = 0x5a5a;
	LONGS_EQUAL(0xa5a5, timer_get_cc(PERIPH_TIMER0, TIMER_CC_0));
	LONGS_EQUAL(0x5a5a, timer_get_cc(PERIPH_TIMER0, TIMER_CC_1));
}

TEST(Timer, get_cc_ShouldReturnZero_WhenUnsupportedChannelGiven) {
	LONGS_EQUAL(0, timer_get_cc(PERIPH_TIMER2, TIMER_CC_0));
}

TEST(Timer, set_counter_ShouldSetCnt) {
	timer_set_counter(PERIPH_TIMER0, 1234);
	LONGS_EQUAL(1234, T0->CNT);
}

TEST(Timer, reset_ShouldClearAllRegs) {
	T0->CON = T0->CMD = T0->GRA = T0->GRB = T0->PRS = T0->CNT = 0xa5a5a5a5;
	timer_reset(PERIPH_TIMER0);
	LONGS_EQUAL(0x7000, T0->CON);
	LONGS_EQUAL(0, T0->CMD);
	LONGS_EQUAL(0, T0->GRA);
	LONGS_EQUAL(0, T0->GRB);
	LONGS_EQUAL(0, T0->PRS);
	LONGS_EQUAL(0, T0->CNT);
}

TEST(Timer, get_event_ShouldReturnCurrentFlags) {
	T0->CON = 0x3000;
	LONGS_EQUAL(TIMER_EVENT_CC_0 | TIMER_EVENT_CC_1,
			timer_get_event(PERIPH_TIMER0));
	T0->CON = 0x4000;
	LONGS_EQUAL(TIMER_EVENT_OVERFLOW, timer_get_event(PERIPH_TIMER0));
}

TEST(Timer, set_polarity_ShouldSetTstrt) {
	timer_set_cc_pin_polarity(PERIPH_TIMER0, TIMER_CC_0, 1);
	LONGS_EQUAL(0x80, T0->CON);
	timer_set_cc_pin_polarity(PERIPH_TIMER0, TIMER_CC_0, 0);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, get_frequency_ShouldReturnTclk_WhenTCSIs4) {
	uint32_t tclk = 12345;
	T0->CON = 4U << 4;
	LONGS_EQUAL(tclk, timer_get_frequency(PERIPH_TIMER0, tclk));
}
TEST(Timer, get_frequency_ShouldReturnTclkDividedBy16_WhenTCSIs2) {
	uint32_t tclk = 6000;
	T0->CON = 2U << 4;
	LONGS_EQUAL(tclk/16, timer_get_frequency(PERIPH_TIMER0, tclk));
}
TEST(Timer, get_frequency_ShouldReturnZero_WhenTCSIs6) {
	uint32_t tclk = 6000;
	T0->CON = 6U << 4;
	LONGS_EQUAL(0, timer_get_frequency(PERIPH_TIMER0, tclk));
}
