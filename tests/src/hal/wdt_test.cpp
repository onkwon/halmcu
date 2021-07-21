#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/hal/wdt.h"

TEST_GROUP(Watchdog) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Watchdog, enable_ShouldEnablePeripheralAndClock) {
	mock().expectOneCall("pwr_enable_peripheral")
		.withParameter("peri", PERIPH_WDT);
	mock().expectOneCall("clk_enable_peripheral")
		.withParameter("peri", PERIPH_WDT);
	wdt_enable();
}

TEST(Watchdog, disable_ShouldDisablePeripheralAndClock) {
	mock().expectOneCall("pwr_disable_peripheral")
		.withParameter("peri", PERIPH_WDT);
	mock().expectOneCall("clk_disable_peripheral")
		.withParameter("peri", PERIPH_WDT);
	wdt_disable();
}

TEST(Watchdog, get_clock_freq_ShouldReturnWdtClockFreq_WhenMainSourceGiven) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_PLL);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_PLL)
		.andReturnValue(75000000);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(64);
	LONGS_EQUAL(1171875, wdt_get_clock_frequency());
}
TEST(Watchdog, get_clock_freq_ShouldReturnWdtClockFreq_WhenIOSC16Given) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_HSI);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_HSI)
		.andReturnValue(16000000);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(16);
	LONGS_EQUAL(1000000, wdt_get_clock_frequency());
}
TEST(Watchdog, get_clock_freq_ShouldReturnWdtClockFreq_WhenSubXtalGiven) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_LSE);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_LSE)
		.andReturnValue(36768);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(1);
	LONGS_EQUAL(36768, wdt_get_clock_frequency());
}
TEST(Watchdog, get_clock_freq_ShouldReturnWdtClockFreq_WheniRingOSCGiven) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_LSI);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_LSI)
		.andReturnValue(1000000);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(256);
	LONGS_EQUAL(3906, wdt_get_clock_frequency());
}
