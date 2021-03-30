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

TEST(Power, t) {
}
