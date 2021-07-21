#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/wdt.h"
#include "a33g.h"

extern "C" {
static struct wdt_stub wdtreg;
static struct pmu_stub pmureg;
struct wdt_stub * const WDT = &wdtreg;
struct pmu_stub * const PMU = &pmureg;
}

TEST_GROUP(Watchdog) {
	void setup(void) {
		mock().ignoreOtherCalls();

		set_initial_state();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}

	void set_initial_state(void) {
		WDT->LR = 0;
		WDT->CVR = 0xffff;
		WDT->CON = 0x47;
	}
};

TEST(Watchdog, set_prescaler_ShouldSetPrescaler) {
	wdt_set_prescaler(1);
	LONGS_EQUAL(0, WDT->CON & 7);
	wdt_set_prescaler(4);
	LONGS_EQUAL(1, WDT->CON & 7);
	wdt_set_prescaler(8);
	LONGS_EQUAL(2, WDT->CON & 7);
	wdt_set_prescaler(16);
	LONGS_EQUAL(3, WDT->CON & 7);
	wdt_set_prescaler(256);
	LONGS_EQUAL(7, WDT->CON & 7);
}

TEST(Watchdog, get_prescaler_ShouldReturnDivFactor) {
	WDT->CON = 0;
	LONGS_EQUAL(1, wdt_get_prescaler());
	WDT->CON = 1;
	LONGS_EQUAL(4, wdt_get_prescaler());
	WDT->CON = 2;
	LONGS_EQUAL(8, wdt_get_prescaler());
	WDT->CON = 3;
	LONGS_EQUAL(16, wdt_get_prescaler());
	WDT->CON = 7;
	LONGS_EQUAL(256, wdt_get_prescaler());
}

TEST(Watchdog, reload_ShouldReloadTimeoutCounter) {
	wdt_set_reload(1);
	LONGS_EQUAL(1, WDT->LR);
	wdt_set_reload(0xffffffff);
	LONGS_EQUAL(0xffffffff, WDT->LR);
}

TEST(Watchdog, get_reload_ShouldReturnReloadTimeout) {
	WDT->LR = 1000;
	LONGS_EQUAL(1000, wdt_get_reload());
}

TEST(Watchdog, start_ShouldSetWEN) {
	wdt_start();
	LONGS_EQUAL(0x67, WDT->CON);
}

TEST(Watchdog, stop_ShouldClearWEN) {
	wdt_start();
	wdt_stop();
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, set_debug_ShouldSetWDH_WhenTrueGiven) {
	wdt_set_debug_stop_mode(true);
	LONGS_EQUAL(0x8047, WDT->CON);
}

TEST(Watchdog, set_debug_ShouldClearWDH_WhenfalseGiven) {
	wdt_set_debug_stop_mode(true);
	wdt_set_debug_stop_mode(false);
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, isEventRaised_ShouldReturnFalse_WhenNoInterruptDelivered) {
	LONGS_EQUAL(0, wdt_is_event_raised());
}

TEST(Watchdog, isEventRaised_ShouldReturnTrue_WhenInterruptDelivered) {
	WDT->CON |= 1UL << 8;
	LONGS_EQUAL(1, wdt_is_event_raised());
}

TEST(Watchdog, set_interrupt_ShouldSetWIE_WhenTrueGiven) {
	wdt_set_interrupt(true);
	LONGS_EQUAL(0x87, WDT->CON);
}

TEST(Watchdog, set_interrupt_ShouldSetWRE_WhenTrueGiven) {
	wdt_set_interrupt(true);
	wdt_set_interrupt(false);
	LONGS_EQUAL(0x47, WDT->CON);
}

TEST(Watchdog, get_count_ShouldReturnCVR) {
	LONGS_EQUAL(0xffff, wdt_get_count());
}

#include "halmcu/hal/wdt.h"
#include "halmcu/periph/clk.h"

TEST(Watchdog, set_clock_source_ShouldSetPmuPcsr) {
	PMU->PCSR = 1;
	WDT->CON = 0;
	wdt_set_clock_source(CLK_PLL);
	LONGS_EQUAL(0, PMU->PCSR);
	LONGS_EQUAL(8, WDT->CON);
	wdt_set_clock_source(CLK_HSI);
	LONGS_EQUAL(1, PMU->PCSR);
	wdt_set_clock_source(CLK_LSE);
	LONGS_EQUAL(2, PMU->PCSR);
	wdt_set_clock_source(CLK_LSI);
	LONGS_EQUAL(3, PMU->PCSR);
}
TEST(Watchdog, set_clock_source_ShouldIgnoreParam_WhenInvalidGiven) {
	PMU->PCSR = 0x1000;
	wdt_set_clock_source(CLK_PLL_BYPASS);
	LONGS_EQUAL(0x1000, PMU->PCSR);
}

TEST(Watchdog, get_clock_source_ShouldReturnClockSource) {
	PMU->PCSR = 1;
	LONGS_EQUAL(CLK_HSI, wdt_get_clock_source());
	PMU->PCSR = 2;
	LONGS_EQUAL(CLK_LSE, wdt_get_clock_source());
	PMU->PCSR = 3;
	LONGS_EQUAL(CLK_LSI, wdt_get_clock_source());
	PMU->PCSR = 0;
	LONGS_EQUAL(CLK_PLL, wdt_get_clock_source());
}

TEST(Watchdog, wdt_feed_ShouldSetLR) {
	wdt_feed();
}

TEST(Watchdog, set_reload_ms_ShouldSetPrescalerAndReload) {
	wdt_set_reload_ms(1000);
	LONGS_EQUAL(7, WDT->CON & 0x7);
	LONGS_EQUAL(3000, WDT->LR);
	wdt_set_reload_ms(30000);
	LONGS_EQUAL(7, WDT->CON & 0x7);
	LONGS_EQUAL(90000, WDT->LR);
	wdt_set_reload_ms(1);
	LONGS_EQUAL(7, WDT->CON & 0x7);
	LONGS_EQUAL(3, WDT->LR);
	wdt_set_reload_ms(0);
	LONGS_EQUAL(0, WDT->CON & 0x7);
	LONGS_EQUAL(0, WDT->LR);
}
