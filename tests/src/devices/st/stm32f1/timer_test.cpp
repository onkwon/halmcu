#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/timer.h"
#include "stm32f1.h"
#include "abov/assert.h"

extern "C" {
static RCC_Type rcc_reg;
static TIM_Type regTim1, regTim2, regTim3, regTim4,
		regTim5, regTim6, regTim7, regTim8;
RCC_Type * const RCC = &rcc_reg;
TIM_Type * const TIM1 = &regTim1;
TIM_Type * const TIM2 = &regTim2;
TIM_Type * const TIM3 = &regTim3;
TIM_Type * const TIM4 = &regTim4;
TIM_Type * const TIM5 = &regTim5;
TIM_Type * const TIM6 = &regTim6;
TIM_Type * const TIM7 = &regTim7;
TIM_Type * const TIM8 = &regTim8;
}

TEST_GROUP(Timer) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(TIM1, 0, sizeof(*TIM1));
		memset(TIM2, 0, sizeof(*TIM2));
		memset(TIM3, 0, sizeof(*TIM3));
		memset(TIM4, 0, sizeof(*TIM4));
		memset(TIM5, 0, sizeof(*TIM5));
		memset(TIM6, 0, sizeof(*TIM6));
		memset(TIM7, 0, sizeof(*TIM7));
		memset(TIM8, 0, sizeof(*TIM8));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Timer, reset_ShouldSetAndClearRSTR_WhenTIM1Given) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 11)
		.ignoreOtherParameters();
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 11)
		.ignoreOtherParameters();
	timer_ll_reset(PERIPH_TIM1);
}
TEST(Timer, reset_ShouldSetAndClearRSTR_WhenTIM2Given) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 0)
		.ignoreOtherParameters();
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 0)
		.ignoreOtherParameters();
	timer_ll_reset(PERIPH_TIM2);
}
TEST(Timer, reset_ShouldSetAndClearRSTR_WhenTIM7Given) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 5)
		.ignoreOtherParameters();
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 5)
		.ignoreOtherParameters();
	timer_ll_reset(PERIPH_TIM7);
}

TEST(Timer, set_prescaler_ShouldSetPSC) {
	timer_ll_set_prescaler(PERIPH_TIM7, 0x1234);
	LONGS_EQUAL(0x1234, TIM7->PSC);
}

TEST(Timer, set_reload_ShouldSetARR) {
	timer_ll_set_reload(PERIPH_TIM3, 0x1234);
	LONGS_EQUAL(0x1234, TIM3->ARR);
}
TEST(Timer, get_reload_ShouldReturnARR) {
	TIM4->ARR = 0xfeed;
	LONGS_EQUAL(0xfeed, timer_ll_get_reload(PERIPH_TIM4));
}

TEST(Timer, start_ShouldSetCEN) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 0)
		.ignoreOtherParameters();
	timer_ll_start(PERIPH_TIM1);
	LONGS_EQUAL(1, TIM1->CR1);
}
TEST(Timer, stop_ShouldClearCEN) {
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 0)
		.ignoreOtherParameters();
	timer_ll_stop(PERIPH_TIM2);
}
