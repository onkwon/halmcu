#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/asm/arm/systick.h"
#include "ARMCM3.h"

extern "C" {
static struct systick stkreg;
struct systick * const SysTick = &stkreg;
}

TEST_GROUP(systick) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(SysTick, 0, sizeof(*SysTick));
		SysTick->CTRL = 0x4;
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(systick, start_ShouldSetCtrlRegister) {
	systick_start();
	LONGS_EQUAL(4 | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk, SysTick->CTRL);
}
TEST(systick, stop_ShouldClearCtrlRegister) {
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
	systick_stop();
	LONGS_EQUAL(4, SysTick->CTRL);
}
TEST(systick, clear_ShouldClearCounter) {
	SysTick->VAL = SysTick_LOAD_RELOAD_Msk;
	systick_clear();
	LONGS_EQUAL(0, SysTick->VAL);
}
TEST(systick, get_frequency_ShouldReturnSysTickFrequency) {
	mock().expectOneCall("clk_ll_get_hclk_frequency").andReturnValue(1000000);
	SysTick->LOAD = 20000 - 1;
	LONGS_EQUAL(50, systick_get_frequency());
}
TEST(systick, set_frequency_ShouldSetSysTickFrequency_When1KhzGiven) {
	mock().expectOneCall("clk_ll_get_hclk_frequency").andReturnValue(1000000);
	LONGS_EQUAL(1000, systick_set_frequency(1000));
	LONGS_EQUAL(1000-1, SysTick->LOAD);
}
TEST(systick, set_frequency_ShouldSetSysTickFrequency_When100HzGiven) {
	mock().expectOneCall("clk_ll_get_hclk_frequency").andReturnValue(1000000);
	LONGS_EQUAL(100, systick_set_frequency(100));
	LONGS_EQUAL(10000-1, SysTick->LOAD);
}
TEST(systick, set_frequency_ShouldSetSysTickFrequency_When50HzGiven) {
	mock().expectOneCall("clk_ll_get_hclk_frequency").andReturnValue(1000000);
	LONGS_EQUAL(50, systick_set_frequency(50));
	LONGS_EQUAL(20000-1, SysTick->LOAD);
}
TEST(systick, set_frequency_ShouldSetSysTickFrequency_When1HzGiven) {
	mock().expectOneCall("clk_ll_get_hclk_frequency").andReturnValue(1000000);
	LONGS_EQUAL(1, systick_set_frequency(1));
	LONGS_EQUAL(1000000-1, SysTick->LOAD);
}
