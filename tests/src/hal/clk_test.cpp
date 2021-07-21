#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/hal/clk.h"

TEST_GROUP(CLK) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(CLK, init_ShouldReturnFalse_WhenSettingPLLFailed) {
	uint32_t fixed_hz = 1000000;
	mock().expectOneCall("clk_ll_set_pll_frequency")
		.withParameter("clk", CLK_PLL)
		.withParameter("clkin", CLK_HSI)
		.withParameter("hz", fixed_hz)
		.andReturnValue(false);

	LONGS_EQUAL(false, clk_init(CLK_HSI, fixed_hz));
}

TEST(CLK, init_ShouldCallEnableSource) {
	mock().expectOneCall("clk_ll_enable_source").withParameter("clk", CLK_HSI);
	clk_init(CLK_HSI, 1000000);
	mock().expectOneCall("clk_ll_enable_source").withParameter("clk", CLK_HSE);
	clk_init(CLK_HSE, 1000000);
}

TEST(CLK, init_ShouldCallStartPllAndWaitForItToBeLocked) {
	mock().expectOneCall("clk_ll_start_pll");
	mock().expectOneCall("clk_ll_is_pll_locked").andReturnValue(true);
	clk_init(CLK_HSI, 1000000);
}

TEST(CLK, init_ShouldSetPLLAsSystemClockSource) {
	mock().expectOneCall("clk_ll_set_source").withParameter("clk", CLK_PLL);
	clk_init(CLK_HSI, 1000000);
}
