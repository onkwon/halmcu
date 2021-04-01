#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/uart.h"
#include "a33g.h"

extern "C" {
}

TEST_GROUP(uart) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void)
	{
	}
};

TEST(uart, t) {
}
