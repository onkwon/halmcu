#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/timer.h"
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
	timer_set_prescaler(PERIPHERAL_TIMER0, 123);
	LONGS_EQUAL(123, T0->PRS);
	timer_set_prescaler(PERIPHERAL_TIMER3, 2);
	LONGS_EQUAL(2, T3->PRS);
	timer_set_prescaler(PERIPHERAL_TIMER9, 16);
	LONGS_EQUAL(16, T9->PRS);
}

TEST(Timer, set_divider_ShouldSetTimerClockSource) {
	timer_set_divider(PERIPHERAL_TIMER0, 4);
	LONGS_EQUAL(0x40, T0->CON);
	timer_set_divider(PERIPHERAL_TIMER0, 6);
	LONGS_EQUAL(0x60, T0->CON);
	timer_set_divider(PERIPHERAL_TIMER0, 1);
	LONGS_EQUAL(0x10, T0->CON);
	timer_set_divider(PERIPHERAL_TIMER0, 0);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, get_counter_ShouldReturnCntValue) {
	T0->CNT = 123;
	LONGS_EQUAL(123, timer_get_counter(PERIPHERAL_TIMER0));
	T4->CNT = 1;
	LONGS_EQUAL(1, timer_get_counter(PERIPHERAL_TIMER4));
	T8->CNT = 2;
	LONGS_EQUAL(2, timer_get_counter(PERIPHERAL_TIMER8));
}

TEST(Timer, set_reload_ShouldSetGra) {
	timer_set_reload(PERIPHERAL_TIMER0, 123);
	LONGS_EQUAL(123, T0->GRA);
	timer_set_reload(PERIPHERAL_TIMER2, 3);
	LONGS_EQUAL(3, T2->GRA);
	timer_set_reload(PERIPHERAL_TIMER5, 4);
	LONGS_EQUAL(4, T5->GRA);
}

TEST(Timer, get_reload_ShouldReturnGra) {
	T0->GRA = 123;
	LONGS_EQUAL(123, timer_get_reload(PERIPHERAL_TIMER0));
	T6->GRA = 5;
	LONGS_EQUAL(5, timer_get_reload(PERIPHERAL_TIMER6));
	T7->GRA = 6;
	LONGS_EQUAL(6, timer_get_reload(PERIPHERAL_TIMER7));
}

TEST(Timer, set_mode_ShouldSetConMode) {
	timer_set_mode(PERIPHERAL_TIMER0, TIMER_MODE_PWM);
	LONGS_EQUAL(1, T0->CON);
	timer_set_mode(PERIPHERAL_TIMER0, TIMER_MODE_INPUT_CAPTURE);
	LONGS_EQUAL(3, T0->CON);
	timer_set_mode(PERIPHERAL_TIMER0, TIMER_MODE_ONESHOT);
	LONGS_EQUAL(2, T0->CON);
	timer_set_mode(PERIPHERAL_TIMER0, TIMER_MODE_NORMAL);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, enable_irq_ShouldSetIrqFlags) {
	timer_enable_irq(PERIPHERAL_TIMER0, TIMER_IRQ_OVERFLOW);
	LONGS_EQUAL(0x400, T0->CON);
	timer_enable_irq(PERIPHERAL_TIMER0, TIMER_IRQ_COMPARE_0);
	LONGS_EQUAL(0x500, T0->CON);
	timer_enable_irq(PERIPHERAL_TIMER0, TIMER_IRQ_COMPARE_1);
	LONGS_EQUAL(0x700, T0->CON);
}

TEST(Timer, enable_irq_ShouldDoNothing_WhenUnsupportedGiven) {
	timer_enable_irq(PERIPHERAL_TIMER0, TIMER_IRQ_COMPARE_2);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, disable_irq_ShouldClearIrqFlags) {
	T1->CON = 0x700;
	timer_disable_irq(PERIPHERAL_TIMER1, TIMER_IRQ_OVERFLOW);
	LONGS_EQUAL(0x300, T1->CON);
	timer_disable_irq(PERIPHERAL_TIMER1, TIMER_IRQ_COMPARE_0);
	LONGS_EQUAL(0x200, T1->CON);
	timer_disable_irq(PERIPHERAL_TIMER1, TIMER_IRQ_COMPARE_1);
	LONGS_EQUAL(0, T1->CON);
}

TEST(Timer, clear_event_ShouldClearIrqFlag) {
	timer_clear_event(PERIPHERAL_TIMER0, TIMER_IRQ_OVERFLOW);
	LONGS_EQUAL(0x4000, T0->CON);
	timer_clear_event(PERIPHERAL_TIMER0, TIMER_IRQ_COMPARE_0);
	LONGS_EQUAL(0x5000, T0->CON);
	timer_clear_event(PERIPHERAL_TIMER0, TIMER_IRQ_COMPARE_1);
	LONGS_EQUAL(0x7000, T0->CON);
}

TEST(Timer, start_ShouldSetTEN) {
	timer_start(PERIPHERAL_TIMER0);
	LONGS_EQUAL(1, T0->CMD);
}

TEST(Timer, stop_ShouldClearTEN) {
	T0->CMD = 1;
	timer_stop(PERIPHERAL_TIMER0);
	LONGS_EQUAL(0, T0->CMD);
}

TEST(Timer, set_compare_ShouldSetCompareRegister) {
	timer_set_compare(PERIPHERAL_TIMER0, 0, 1234);
	LONGS_EQUAL(1234, T0->GRA);
	timer_set_compare(PERIPHERAL_TIMER1, 1, 5678);
	LONGS_EQUAL(5678, T1->GRB);
}

TEST(Timer, set_counter_ShouldSetCnt) {
	timer_set_counter(PERIPHERAL_TIMER0, 1234);
	LONGS_EQUAL(1234, T0->CNT);
}

TEST(Timer, reset_ShouldClearAllRegs) {
	T0->CON = T0->CMD = T0->GRA = T0->GRB = T0->PRS = T0->CNT = 0xa5a5a5a5;
	timer_reset(PERIPHERAL_TIMER0);
	LONGS_EQUAL(0x700, T0->CON);
	LONGS_EQUAL(0, T0->CMD);
	LONGS_EQUAL(0, T0->GRA);
	LONGS_EQUAL(0, T0->GRB);
	LONGS_EQUAL(0, T0->PRS);
	LONGS_EQUAL(0, T0->CNT);
}

TEST(Timer, get_event_ShouldReturnCurrentFlags) {
	T0->CON = 0x3000;
	LONGS_EQUAL(TIMER_IRQ_COMPARE_0 | TIMER_IRQ_COMPARE_1,
			timer_get_event(PERIPHERAL_TIMER0));
	T0->CON = 0x4000;
	LONGS_EQUAL(TIMER_IRQ_OVERFLOW, timer_get_event(PERIPHERAL_TIMER0));
}

TEST(Timer, set_polarity_ShouldSetTstrt) {
	timer_set_polarity(PERIPHERAL_TIMER0, 1);
	LONGS_EQUAL(0x80, T0->CON);
	timer_set_polarity(PERIPHERAL_TIMER0, 0);
	LONGS_EQUAL(0, T0->CON);
}

TEST(Timer, set_edge_ShouldSetCapm) {
	timer_set_edge(PERIPHERAL_TIMER0, TIMER_FALLING_EDGE);
	LONGS_EQUAL(8, T0->CON);
	timer_set_edge(PERIPHERAL_TIMER0, TIMER_RISING_EDGE);
	LONGS_EQUAL(0, T0->CON);
}
