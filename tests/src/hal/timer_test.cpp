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

TEST_GROUP(Timer) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Timer, init_ShouldReturnFalse_WhenInvalidPeripheralGiven) {
}
TEST(Timer, init_ShouldReturnFalse_WhenNullConfigGiven) {
}
TEST(Timer, init_ShouldEnablePowerAndClock) {
}
TEST(Timer, init_ShouldSetTimerMode) {
}
TEST(Timer, init_ShouldSetTimerPrescaler_WhenNotCaptureMode) {
}
TEST(Timer, init_ShouldEnableTimerIRQ_WhenIRQOptionGiven) {
}
TEST(Timer, init_ShouldSetIRQPriority_WhenIRQOptionGiven) {
}
