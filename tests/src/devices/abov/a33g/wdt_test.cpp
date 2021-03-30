#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/wdt.h"
#include "a33g.h"

extern "C" {
static struct wdt wdtreg;
struct wdt * const WDT = &wdtreg;
}

TEST_GROUP(Watchdog) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void)
	{
		WDT->LR = 0;
		WDT->CVR = 0xffff;
		WDT->CON = 0x47;
	}
};

TEST(Watchdog, set_prescaler_ShouldSetPrescaler) {
	for (uint32_t i = 0; i <= 7; i++) {
		wdt_ll_set_prescaler(i);
		LONGS_EQUAL(0x40 | i, WDT->CON);
	}
}

TEST(Watchdog, set_prescaler_ShouldIgnoreOutBoundValue_WhenUnsupportedDivFactorGiven) {
	wdt_ll_set_prescaler(8);
	LONGS_EQUAL(0x40, WDT->CON);
	wdt_ll_set_prescaler(9);
	LONGS_EQUAL(0x41, WDT->CON);
}

TEST(Watchdog, reload_ShouldReloadTimeoutCounter) {
	wdt_ll_reload(1);
	LONGS_EQUAL(1, WDT->LR);
	wdt_ll_reload(0xffffffff);
	LONGS_EQUAL(0xffffffff, WDT->LR);
}

TEST(Watchdog, start_ShouldSetWEN) {
	wdt_ll_start();
	LONGS_EQUAL(0x67, WDT->CON);
}

TEST(Watchdog, stop_ShouldClearWEN) {
	wdt_ll_start();
	wdt_ll_stop();
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, set_debug_ShouldSetWDH_WhenTrueGiven) {
	wdt_ll_set_debug_hold_mode(true);
	LONGS_EQUAL(0x8047, WDT->CON);
}

TEST(Watchdog, set_debug_ShouldClearWDH_WhenfalseGiven) {
	wdt_ll_set_debug_hold_mode(true);
	wdt_ll_set_debug_hold_mode(false);
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, isEventRaised_ShouldReturnFalse_WhenNoInterruptDelivered) {
	LONGS_EQUAL(0, wdt_ll_is_event_raised());
}

TEST(Watchdog, isEventRaised_ShouldReturnTrue_WhenInterruptDelivered) {
	WDT->CON |= 1UL << 8;
	LONGS_EQUAL(1, wdt_ll_is_event_raised());
}

TEST(Watchdog, set_interrupt_ShouldSetWIE_WhenTrueGiven) {
	wdt_ll_set_interrupt(true);
	LONGS_EQUAL(0x87, WDT->CON);
}

TEST(Watchdog, set_interrupt_ShouldSetWRE_WhenTrueGiven) {
	wdt_ll_set_interrupt(true);
	wdt_ll_set_interrupt(false);
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, get_count_ShouldReturnCVR) {
	LONGS_EQUAL(0xffff, wdt_ll_get_count());
}
