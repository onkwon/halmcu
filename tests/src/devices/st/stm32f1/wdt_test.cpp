#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "halmcu/periph/wdt.h"
#include "stm32f1.h"
#include "halmcu/assert.h"

extern "C" {
static IWDT_Type iwdt_reg;
static DBGMCU_Type dbg_reg;
IWDT_Type * const IWDT = &iwdt_reg;
DBGMCU_Type * const DBGMCU = &dbg_reg;
}

TEST_GROUP(IWDT) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(IWDT, 0, sizeof(*IWDT));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(IWDT, set_prescaler_ShouldCauseAssert_WhenZeroValueGiven) {
	mock().expectOneCall("halmcu_assertion_failed");
	wdt_set_prescaler(0);
}
TEST(IWDT, set_prescaler_ShouldCauseAssert_WhenTooBigValueGiven) {
	mock().expectOneCall("halmcu_assertion_failed");
	wdt_set_prescaler(512);
}
TEST(IWDT, set_prescaler_ShouldCauseAssert_WhenTooSmallValueGiven) {
	mock().expectOneCall("halmcu_assertion_failed");
	wdt_set_prescaler(2);
}
TEST(IWDT, set_prescaler_ShouldCauseAssert_WhenNotPowerOfTwoGiven) {
	mock().expectOneCall("halmcu_assertion_failed");
	wdt_set_prescaler(10);
}
TEST(IWDT, set_prescaler_ShouldSetPR) {
	wdt_set_prescaler(4);
	LONGS_EQUAL(0, IWDT->PR);
	wdt_set_prescaler(8);
	LONGS_EQUAL(1, IWDT->PR);
	wdt_set_prescaler(16);
	LONGS_EQUAL(2, IWDT->PR);
	wdt_set_prescaler(256);
	LONGS_EQUAL(6, IWDT->PR);
}
TEST(IWDT, set_prescaler_ShouldSetKR) {
	wdt_set_prescaler(4);
	LONGS_EQUAL(0x5555, IWDT->KR);
}

TEST(IWDT, get_prescaler_ShouldReturnPR) {
	IWDT->PR = 0;
	LONGS_EQUAL(4, wdt_get_prescaler());
	IWDT->PR = 1;
	LONGS_EQUAL(8, wdt_get_prescaler());
	IWDT->PR = 2;
	LONGS_EQUAL(16, wdt_get_prescaler());
	IWDT->PR = 5;
	LONGS_EQUAL(128, wdt_get_prescaler());
}

TEST(IWDT, set_reload_ShouldSetMaximum_WhenTooBigValueGiven) {
	wdt_set_reload(0x1000);
	LONGS_EQUAL(0xfff, IWDT->RLP);
}
TEST(IWDT, set_reload_ShouldSetKR) {
	wdt_set_reload(1);
	LONGS_EQUAL(0x5555, IWDT->KR);
}
TEST(IWDT, set_reload_ShouldSetRLP) {
	wdt_set_reload(0);
	LONGS_EQUAL(0, IWDT->RLP);
	wdt_set_reload(1);
	LONGS_EQUAL(1, IWDT->RLP);
	wdt_set_reload(0x123);
	LONGS_EQUAL(0x123, IWDT->RLP);
}
TEST(IWDT, get_reload_ShouldReturnRLP) {
	IWDT->RLP = 0xA5;
	LONGS_EQUAL(0xA5, wdt_get_reload());
}

TEST(IWDT, feed_ShouldSetKR) {
	wdt_feed();
	LONGS_EQUAL(0xAAAA, IWDT->KR);
}

TEST(IWDT, start_ShouldSetKR) {
	wdt_start();
	LONGS_EQUAL(0xCCCC, IWDT->KR);
}

TEST(IWDT, set_debug_stop_mode_ShouldSetDebugStopMode_WhenEnabled) {
	wdt_set_debug_stop_mode(true);
	LONGS_EQUAL(0x100, DBGMCU->CR);
}
TEST(IWDT, set_debug_stop_mode_ShouldReSetDebugStopMode_WhenDisabled) {
	wdt_set_debug_stop_mode(true);
	wdt_set_debug_stop_mode(false);
	LONGS_EQUAL(0, DBGMCU->CR);
}

TEST(IWDT, get_clock_source_ShouldReturnLSI) {
	LONGS_EQUAL(CLK_LSI, wdt_get_clock_source());
}

TEST(IWDT, set_reload_ms_ShouldSetPrescalerAndReload) {
	wdt_set_reload_ms(1000);
	LONGS_EQUAL(2, IWDT->PR);
	LONGS_EQUAL(2500, IWDT->RLP);
	wdt_set_reload_ms(30000);
	LONGS_EQUAL(7, IWDT->PR);
	LONGS_EQUAL(0xfff, IWDT->RLP);
	wdt_set_reload_ms(1);
	LONGS_EQUAL(0, IWDT->PR);
	LONGS_EQUAL(10, IWDT->RLP);
	wdt_set_reload_ms(0);
	LONGS_EQUAL(0, IWDT->PR);
	LONGS_EQUAL(0, IWDT->RLP);
}
