#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/pwr.h"
#include "a33g.h"

extern "C" {
static struct pmu reg;
struct pmu * const PMU = &reg;
}

TEST_GROUP(Power) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void)
	{
		memset(PMU, 0, sizeof(*PMU));
		PMU->IDR = 0xCEDA0000;
		PMU->RSER = 0x69;
		PMU->PRER = 0xFFFFFFFF;
		PMU->PER = 0xFFFFFFFF;
		PMU->LVDCON = 0x8800;
		PMU->VDCCON = 0xFF;
	}
};

TEST(Power, reboot_ShouldSetSWRST) {
	pwr_ll_reboot();
	LONGS_EQUAL(1, PMU->CFG);
}

TEST(Power, get_reboot_source_ShouldReturnResetSource) {
	PMU->RSSR = 0x40;
	LONGS_EQUAL(0x40, pwr_ll_get_reboot_source());
	PMU->RSSR = 0x8;
	LONGS_EQUAL(0x8, pwr_ll_get_reboot_source());
}

TEST(Power, clear_reboot_source_ShouldClearResetSource) {
	PMU->RSSR = 0xFB;
	pwr_ll_clear_reboot_source(0x40);
	LONGS_EQUAL(0xBB, PMU->RSSR);
	pwr_ll_clear_reboot_source(0x8);
	LONGS_EQUAL(0xB3, PMU->RSSR);
	pwr_ll_clear_reboot_source(0x20);
	LONGS_EQUAL(0x93, PMU->RSSR);
}

TEST(Power, set_mode_ShouldChangeMode) {
	pwr_ll_set_mode(LL_POWER_MODE_SLEEP);
	LONGS_EQUAL(1, PMU->MR);
	pwr_ll_set_mode(LL_POWER_MODE_DEEP_SLEEP);
	LONGS_EQUAL(2, PMU->MR);
	pwr_ll_set_mode(LL_POWER_MODE_BLACKOUT);
	LONGS_EQUAL(6, PMU->MR);
}

TEST(Power, set_wakeup_source_ShouldSetSource) {
	pwr_ll_set_wakeup_source(LL_PERIPHERAL_GPIO); // GPIOA
	LONGS_EQUAL(0x20, PMU->WSER);
	pwr_ll_set_wakeup_source(LL_PERIPHERAL_GPIO + 1); // GPIOB
	LONGS_EQUAL(0x60, PMU->WSER);
	pwr_ll_set_wakeup_source(LL_PERIPHERAL_GPIO + 5); // GPIOF
	LONGS_EQUAL(0x460, PMU->WSER);
	pwr_ll_set_wakeup_source(LL_PERIPHERAL_MXFAIL); // MXFAIL
	LONGS_EQUAL(0x462, PMU->WSER);
}

TEST(Power, clear_wakeup_source_ShouldClearSource) {
	PMU->WSER = 0x7ff;
	pwr_ll_clear_wakeup_source(LL_PERIPHERAL_GPIO + 1); // GPIOB
	LONGS_EQUAL(0x7bf, PMU->WSER);
	pwr_ll_clear_wakeup_source(LL_PERIPHERAL_LVD); // LVD
	LONGS_EQUAL(0x7be, PMU->WSER);
	pwr_ll_clear_wakeup_source(LL_PERIPHERAL_WDT); // WDT
	LONGS_EQUAL(0x7b6, PMU->WSER);
}
