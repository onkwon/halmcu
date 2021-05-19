#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/flash.h"
#include "stm32f1.h"

extern "C" {
static RCC_Type rcc_reg;
RCC_Type * const RCC = &rcc_reg;
}

TEST_GROUP(FLASH) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(FLASH, t) {
}
