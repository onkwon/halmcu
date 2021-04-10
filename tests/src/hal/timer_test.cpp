#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/timer.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/irq.h"

void irq_enable(irq_t irq) {
	mock().actualCall(__func__).withParameter("irq", irq);
}
void irq_disable(irq_t irq) {
	mock().actualCall(__func__).withParameter("irq", irq);
}
void irq_set_priority(irq_t irq, int priority) {
	mock().actualCall(__func__)
		.withParameter("irq", irq).withParameter("priority", priority);
}
void pwr_enable_peripheral(peripheral_t peri) {
	mock().actualCall(__func__).withParameter("peri", peri);
}
void pwr_disable_peripheral(peripheral_t peri) {
	mock().actualCall(__func__).withParameter("peri", peri);
}
void clk_enable_peripheral(peripheral_t peri) {
	mock().actualCall(__func__).withParameter("peri", peri);
}
void clk_disable_peripheral(peripheral_t peri) {
	mock().actualCall(__func__).withParameter("peri", peri);
}
uint32_t clk_get_frequency(clk_source_t clk) {
	return mock().actualCall(__func__).withParameter("clk", clk)
		.returnUnsignedIntValueOrDefault(1000000);
}
clk_source_t clk_get_peripheral_clock_source(peripheral_t peri) {
	return (clk_source_t)mock().actualCall(__func__)
		.withParameter("peri", peri)
		.returnUnsignedIntValueOrDefault(CLK_PLL);
}
void timer_reset(peripheral_t peri) {
	mock().actualCall(__func__).withParameter("peri", peri);
}
void timer_set_mode(peripheral_t peri, timer_mode_t mode) {
	mock().actualCall(__func__)
		.withParameter("peri", peri).withParameter("mode", mode);
}
uint32_t timer_get_frequency(peripheral_t peri, uint32_t tclk) {
	return (clk_source_t)mock().actualCall(__func__)
		.withParameter("peri", peri).withParameter("tclk", tclk)
		.returnUnsignedIntValueOrDefault(0);
}
void timer_set_prescaler(peripheral_t peri, uint32_t div_factor) {
	mock().actualCall(__func__)
		.withParameter("peri", peri).withParameter("div_factor", div_factor);
}
void timer_enable_irq(peripheral_t peri, timer_event_t events) {
	mock().actualCall(__func__)
		.withParameter("peri", peri).withParameter("events", events);
}

TEST_GROUP(Timer) {
	struct timer_cfg default_cfg = {
		.mode = TIMER_MODE_NORMAL,
		.frequency = 1,
		.irq = TIMER_EVENT_NONE,
		.irq_priority = 3,
	};

	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Timer, init_ShouldReturnFalse_WhenInvalidPeripheralGiven) {
	LONGS_EQUAL(0, timer_init(PERIPHERAL_ADC, &default_cfg));
}
TEST(Timer, init_ShouldReturnFalse_WhenNullConfigGiven) {
	LONGS_EQUAL(0, timer_init(PERIPHERAL_TIMER0, NULL));
}
TEST(Timer, init_ShouldEnablePowerAndClock) {
	mock().expectOneCall("pwr_enable_peripheral").withParameter("peri", PERIPHERAL_TIMER0);
	mock().expectOneCall("clk_enable_peripheral").withParameter("peri", PERIPHERAL_TIMER0);
	LONGS_EQUAL(1, timer_init(PERIPHERAL_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldSetTimerMode) {
	mock().expectOneCall("timer_set_mode")
		.withParameter("peri", PERIPHERAL_TIMER0)
		.withParameter("mode", TIMER_MODE_CAPTURE);
	default_cfg.mode = TIMER_MODE_CAPTURE;
	LONGS_EQUAL(1, timer_init(PERIPHERAL_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldNotSetTimerPrescaler_WhenCaptureMode) {
	mock().expectNoCall("timer_set_prescaler");
	default_cfg.mode = TIMER_MODE_CAPTURE;
	LONGS_EQUAL(1, timer_init(PERIPHERAL_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldSetTimerPrescaler_WhenNotCaptureMode) {
	mock().expectOneCall("timer_get_frequency")
		.withParameter("peri", PERIPHERAL_TIMER0)
		.withParameter("tclk", 1000000)
		.andReturnValue(1000);
	mock().expectOneCall("timer_set_prescaler")
		.withParameter("peri", PERIPHERAL_TIMER0)
		.withParameter("div_factor", 1000 - 1);
	default_cfg.frequency = 1;//hz
	timer_init(PERIPHERAL_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldEnableTimerIRQ_WhenIRQOptionGiven) {
	mock().expectOneCall("timer_enable_irq")
		.withParameter("peri", PERIPHERAL_TIMER0)
		.withParameter("events", TIMER_EVENT_OVERFLOW | TIMER_EVENT_CC_1);
	mock().expectOneCall("irq_enable").withParameter("irq", IRQ_TIMER0);
	default_cfg.irq = (timer_event_t)(TIMER_EVENT_OVERFLOW | TIMER_EVENT_CC_1);
	timer_init(PERIPHERAL_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldSetIRQPriority_WhenIRQOptionGiven) {
	mock().expectOneCall("irq_set_priority")
		.withParameter("irq", IRQ_TIMER0)
		.withParameter("priority", 5);
	default_cfg.irq_priority = 5;
	default_cfg.irq = TIMER_EVENT_OVERFLOW;
	timer_init(PERIPHERAL_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldNotCallIrqRelated_WhenNoEventGiven) {
	mock().expectNoCall("timer_enable_irq");
	mock().expectNoCall("irq_set_priority");
	mock().expectNoCall("irq_enable");
	default_cfg.irq = TIMER_EVENT_NONE;
	timer_init(PERIPHERAL_TIMER0, &default_cfg);
}

TEST(Timer, deinit_ShouldDisablePowerAndClock) {
	mock().expectOneCall("pwr_disable_peripheral").withParameter("peri", PERIPHERAL_TIMER0);
	mock().expectOneCall("clk_disable_peripheral").withParameter("peri", PERIPHERAL_TIMER0);
	timer_deinit(PERIPHERAL_TIMER0);
}
