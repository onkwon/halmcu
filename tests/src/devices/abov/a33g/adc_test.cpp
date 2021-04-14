#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/adc.h"
#include "a33g.h"
#include "abov/peripheral.h"

extern "C" {
static struct adc_stub adcreg;
struct adc_stub * const ADC = &adcreg;
}

TEST_GROUP(ADC) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(ADC, test) {
}
