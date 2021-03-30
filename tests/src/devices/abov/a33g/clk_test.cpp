#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/hal/clk.h"
#include "a33g.h"

extern "C" {
static struct pmu reg;
struct pmu * const PMU = &reg;
}

TEST_GROUP(Clock) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void)
	{
		memset(PMU, 0, sizeof(*PMU));
		PMU->PCCR = 0x118;
		PMU->CCR = 0x80;
		PMU->EOSCCON = 0x1;
	}
};

TEST(Clock, t) {
}
