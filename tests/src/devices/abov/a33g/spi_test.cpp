#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/spi.h"
#include "a33g.h"
#include "abov/peripheral.h"

extern "C" {
static struct spi_stub spireg0, spireg1;
struct spi_stub * const SPI0 = &spireg0;
struct spi_stub * const SPI1 = &spireg1;
}

TEST_GROUP(SPI) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(SPI, test) {
}
