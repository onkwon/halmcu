#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/pwr.h"
#include "a33g.h"

extern "C" {
static struct pmu_stub reg;
struct pmu_stub * const PMU = &reg;
}

TEST_GROUP(Power) {
	void setup(void) {
		pwr_reset();

		PMU->IDR = 0xCEDA0000;
		PMU->PRER = 0xFFFFFFFF;
		PMU->PER = 0xFFFFFFFF;
	}
	void teardown(void) {
	}
};

TEST(Power, reboot_ShouldSetSWRST) {
	pwr_reboot();
	LONGS_EQUAL(1, PMU->CFG);
}

TEST(Power, get_reboot_source_ShouldReturnResetSource) {
	PMU->RSSR = 0x40;
	LONGS_EQUAL(0x40, pwr_get_reboot_source());
	PMU->RSSR = 0x8;
	LONGS_EQUAL(0x8, pwr_get_reboot_source());
}

TEST(Power, clear_reboot_source_ShouldClearResetSource) {
	PMU->RSSR = 0xFB;
	pwr_clear_reboot_source(0x40);
	LONGS_EQUAL(0xBB, PMU->RSSR);
	pwr_clear_reboot_source(0x8);
	LONGS_EQUAL(0xB3, PMU->RSSR);
	pwr_clear_reboot_source(0x20);
	LONGS_EQUAL(0x93, PMU->RSSR);
}

TEST(Power, set_mode_ShouldChangeMode) {
	pwr_set_mode(PWR_MODE_SLEEP);
	LONGS_EQUAL(1, PMU->MR);
	pwr_set_mode(PWR_MODE_DEEP_SLEEP);
	LONGS_EQUAL(2, PMU->MR);
	pwr_set_mode(PWR_MODE_BLACKOUT);
	LONGS_EQUAL(6, PMU->MR);
	pwr_set_mode(PWR_MODE_RUN);
	LONGS_EQUAL(0, PMU->MR);
}

TEST(Power, set_wakeup_source_ShouldSetSource) {
	pwr_set_wakeup_source(PERIPH_GPIOA);
	LONGS_EQUAL(0x20, PMU->WSER);
	pwr_set_wakeup_source(PERIPH_GPIOB);
	LONGS_EQUAL(0x60, PMU->WSER);
	pwr_set_wakeup_source(PERIPH_GPIOF);
	LONGS_EQUAL(0x460, PMU->WSER);
	pwr_set_wakeup_source(PERIPH_MXOSCFAIL);
	LONGS_EQUAL(0x462, PMU->WSER);
}

TEST(Power, clear_wakeup_source_ShouldClearSource) {
	PMU->WSER = 0x7ff;
	pwr_clear_wakeup_source(PERIPH_GPIOB);
	LONGS_EQUAL(0x7bf, PMU->WSER);
	pwr_clear_wakeup_source(PERIPH_LVDFAIL);
	LONGS_EQUAL(0x7be, PMU->WSER);
	pwr_clear_wakeup_source(PERIPH_WDT);
	LONGS_EQUAL(0x7b6, PMU->WSER);
}

TEST(Power, enable_peripheral_ShouldEnablePeripheral) {
	PMU->PER = 0;
	pwr_enable_peripheral(PERIPH_JTAG);
	LONGS_EQUAL(0x80000000, PMU->PER);
	pwr_enable_peripheral(PERIPH_PWM0);
	LONGS_EQUAL(0x81000000, PMU->PER);
	pwr_enable_peripheral(PERIPH_UART1);
	LONGS_EQUAL(0x81200000, PMU->PER);
	pwr_enable_peripheral(PERIPH_I2C0);
	LONGS_EQUAL(0x81240000, PMU->PER);
	pwr_enable_peripheral(PERIPH_GPIOC);
	LONGS_EQUAL(0x81240400, PMU->PER);
}

TEST(Power, disable_peripheral_ShouldDisablePeripheral) {
	pwr_disable_peripheral(PERIPH_GPIOA);
	LONGS_EQUAL(0xfffffeff, PMU->PER);
	pwr_disable_peripheral(PERIPH_GPIOF);
	LONGS_EQUAL(0xffffdeff, PMU->PER);
	pwr_disable_peripheral(PERIPH_UART2);
	LONGS_EQUAL(0xffbfdeff, PMU->PER);
	pwr_disable_peripheral(PERIPH_ADC);
	LONGS_EQUAL(0xefbfdeff, PMU->PER);
	pwr_disable_peripheral(PERIPH_WDT);
	LONGS_EQUAL(0xefbfdef7, PMU->PER);
}

TEST(Power, get_wakeup_source_ShouldReturnWakeupSource) {
	PMU->WSSR = 0x80;
	LONGS_EQUAL(0x80, pwr_get_wakeup_source());
}
