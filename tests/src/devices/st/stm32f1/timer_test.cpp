#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/timer.h"
#include "stm32f1.h"
#include "abov/assert.h"

extern "C" {
static TIM_Type regTim1;
TIM_Type * const TIM1 = &regTim1;
}

TEST_GROUP(Timer) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(TIM1, 0, sizeof(*TIM1));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(Timer, t) {
}
