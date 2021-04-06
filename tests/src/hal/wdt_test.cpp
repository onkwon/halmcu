#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/wdt.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

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
uint32_t wdt_get_prescaler(void) {
	return mock().actualCall(__func__).returnUnsignedIntValueOrDefault(1);
}
clk_source_t wdt_get_clock_source(void) {
	return (clk_source_t)mock().actualCall(__func__).returnUnsignedIntValueOrDefault(CLK_PLL);
}
void wdt_reload(uint32_t timeout) {
	mock().actualCall(__func__).withParameter("timeout", timeout);
}
uint32_t wdt_get_reload(void) {
	return mock().actualCall(__func__).returnUnsignedIntValueOrDefault(0);
}

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
		.withParameter("peri", PERIPHERAL_WDT);
	mock().expectOneCall("clk_enable_peripheral")
		.withParameter("peri", PERIPHERAL_WDT);
	wdt_enable();
}

TEST(Watchdog, disable_ShouldDisablePeripheralAndClock) {
	mock().expectOneCall("pwr_disable_peripheral")
		.withParameter("peri", PERIPHERAL_WDT);
	mock().expectOneCall("clk_disable_peripheral")
		.withParameter("peri", PERIPHERAL_WDT);
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

TEST(Watchdog, set_ShouldSetTimeout_When1SecTimeoutGiven) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_LSI);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_LSI)
		.andReturnValue(1000000);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(1);

	mock().expectOneCall("wdt_reload").withParameter("timeout", 1000000);
	wdt_set_ms(1000);
}
TEST(Watchdog, set_ShouldSetTimeout_When1MilliSecTimeoutGiven) {
	mock().expectOneCall("wdt_get_clock_source").andReturnValue(CLK_LSI);
	mock().expectOneCall("clk_get_frequency").withParameter("clk", CLK_LSI)
		.andReturnValue(1000000);
	mock().expectOneCall("wdt_get_prescaler").andReturnValue(1);

	mock().expectOneCall("wdt_reload").withParameter("timeout", 1000);
	wdt_set_ms(1);
}

TEST(Watchdog, wdt_feed_ShouldReloadWatchdogTimeout) {
	mock().expectOneCall("wdt_get_reload").andReturnValue(1000000);
	mock().expectOneCall("wdt_reload").withParameter("timeout", 1000000);
	wdt_feed();
}