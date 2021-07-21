#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/hal/timer.h"
#include "halmcu/irq.h"

static void set_clock_source(void) {
	mock().actualCall(__func__);
}

TEST_GROUP(Timer) {
	struct timer_cfg default_cfg = {
		.mode = TIMER_MODE_NORMAL,
		.frequency = 1,
		.irq = TIMER_EVENT_NONE,
		.irq_priority = 3,
		.set_clock_source = NULL,
	};

	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

IGNORE_TEST(Timer, init_ShouldReturnFalse_WhenInvalidPeripheralGiven) {
	LONGS_EQUAL(0, timer_init(PERIPH_JTAG, &default_cfg));
}
TEST(Timer, init_ShouldReturnFalse_WhenNullConfigGiven) {
	LONGS_EQUAL(0, timer_init(PERIPH_TIMER0, NULL));
}
TEST(Timer, init_ShouldEnablePowerAndClock) {
	mock().expectOneCall("pwr_ll_enable_peripheral").withParameter("peri", PERIPH_TIMER0);
	mock().expectOneCall("clk_enable_peripheral").withParameter("peri", PERIPH_TIMER0);
	LONGS_EQUAL(1, timer_init(PERIPH_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldSetTimerMode) {
	mock().expectOneCall("timer_ll_set_mode")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("mode", TIMER_MODE_CAPTURE);
	default_cfg.mode = TIMER_MODE_CAPTURE;
	LONGS_EQUAL(1, timer_init(PERIPH_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldNotSetTimerPrescaler_WhenCaptureMode) {
	mock().expectNoCall("timer_ll_set_prescaler");
	default_cfg.mode = TIMER_MODE_CAPTURE;
	LONGS_EQUAL(1, timer_init(PERIPH_TIMER0, &default_cfg));
}
TEST(Timer, init_ShouldSetTimerPrescaler_WhenNotCaptureMode) {
	mock().expectOneCall("clk_get_peripheral_clock_source_frequency")
		.withParameter("peri", PERIPH_TIMER0)
		.andReturnValue(1000000);
	mock().expectOneCall("timer_ll_get_frequency")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("tclk", 1000000)
		.andReturnValue(1000);
	mock().expectOneCall("timer_ll_set_prescaler")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("div_factor", 1000 - 1);
	default_cfg.frequency = 1;//hz
	timer_init(PERIPH_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldEnableTimerIRQ_WhenIRQOptionGiven) {
	mock().expectOneCall("timer_ll_enable_irq")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("events", TIMER_EVENT_OVERFLOW | TIMER_EVENT_CC_1);
	mock().expectOneCall("irq_enable").withParameter("irq", IRQ_TIMER0);
	default_cfg.irq = (timer_event_t)(TIMER_EVENT_OVERFLOW | TIMER_EVENT_CC_1);
	timer_init(PERIPH_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldSetIRQPriority_WhenIRQOptionGiven) {
	mock().expectOneCall("irq_set_priority")
		.withParameter("irq", IRQ_TIMER0)
		.withParameter("priority", 5);
	default_cfg.irq_priority = 5;
	default_cfg.irq = TIMER_EVENT_OVERFLOW;
	timer_init(PERIPH_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldNotCallIrqRelated_WhenNoEventGiven) {
	mock().expectNoCall("timer_ll_enable_irq");
	mock().expectNoCall("irq_set_priority");
	mock().expectNoCall("irq_enable");
	default_cfg.irq = TIMER_EVENT_NONE;
	timer_init(PERIPH_TIMER0, &default_cfg);
}
TEST(Timer, init_ShouldRunCallback_WhenGiven) {
	mock().expectOneCall("set_clock_source");
	default_cfg.set_clock_source = set_clock_source;
	timer_init(PERIPH_TIMER0, &default_cfg);
}

TEST(Timer, deinit_ShouldDisablePowerAndClock) {
	mock().expectOneCall("pwr_ll_disable_peripheral").withParameter("peri", PERIPH_TIMER0);
	mock().expectOneCall("clk_disable_peripheral").withParameter("peri", PERIPH_TIMER0);
	timer_deinit(PERIPH_TIMER0);
}

TEST(Timer, start_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_start")
		.withParameter("peri", PERIPH_TIMER0);
	timer_start(PERIPH_TIMER0);
}
TEST(Timer, stop_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_stop")
		.withParameter("peri", PERIPH_TIMER0);
	timer_stop(PERIPH_TIMER0);
}
TEST(Timer, set_reload_ShouldSetReloadRegister) {
	mock().expectOneCall("timer_ll_set_reload")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("value", 1000);
	timer_set_reload(PERIPH_TIMER0, 1000);
}
TEST(Timer, get_reload_ShouldReturnReloadRegisterValue) {
	mock().expectOneCall("timer_ll_get_reload")
		.withParameter("peri", PERIPH_TIMER0)
		.andReturnValue(1000);
	LONGS_EQUAL(1000, timer_get_reload(PERIPH_TIMER0));
}
TEST(Timer, set_cc_ShouldSetCCRegister) {
	mock().expectOneCall("timer_ll_set_cc")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_0)
		.withParameter("value", 2000);
	timer_set_cc(PERIPH_TIMER0, TIMER_CC_0, 2000);
}
TEST(Timer, get_cc_ShouldReturnCCRegisterValue) {
	mock().expectOneCall("timer_ll_get_cc")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_1)
		.andReturnValue(1234);
	LONGS_EQUAL(1234, timer_get_cc(PERIPH_TIMER0, TIMER_CC_1));
}
TEST(Timer, clear_event_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_clear_event")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("events", TIMER_EVENT_OVERFLOW);
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_OVERFLOW);
}
TEST(Timer, get_event_ShouldReturnEvents) {
	mock().expectOneCall("timer_ll_get_event")
		.withParameter("peri", PERIPH_TIMER0)
		.andReturnValue(TIMER_EVENT_CC_0);
	LONGS_EQUAL(TIMER_EVENT_CC_0, timer_get_event(PERIPH_TIMER0));
}
TEST(Timer, set_polarity_ShouldSetPolarityRegister) {
	mock().expectOneCall("timer_ll_set_cc_pin_polarity")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_1)
		.withParameter("active_high", true);
	timer_set_cc_pin_polarity(PERIPH_TIMER0, TIMER_CC_1, true);
}

TEST(Timer, set_cc_mode_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_set_cc_pin_mode")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_0)
		.withParameter("mode", TIMER_CC_MODE_ACTIVE_LOW);
	timer_set_cc_pin_mode(PERIPH_TIMER0, TIMER_CC_0, TIMER_CC_MODE_ACTIVE_LOW);
}
TEST(Timer, enable_cc_pin_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_enable_cc_pin")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_0);
	timer_enable_cc_pin(PERIPH_TIMER0, TIMER_CC_0);
}
TEST(Timer, disable_cc_pin_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_disable_cc_pin")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_0);
	timer_disable_cc_pin(PERIPH_TIMER0, TIMER_CC_0);
}
TEST(Timer, set_cc_pin_ShouldCallLowLevelFunc) {
	mock().expectOneCall("timer_ll_set_cc_pin")
		.withParameter("peri", PERIPH_TIMER0)
		.withParameter("cc", TIMER_CC_0)
		.withParameter("value", 3);
	timer_set_cc_pin(PERIPH_TIMER0, TIMER_CC_0, 3);
}
