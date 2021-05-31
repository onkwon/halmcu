#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/spi.h"

TEST_GROUP(SPI) {
	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(SPI, enable_ShouldEnablePeripheralAndClock) {
	mock().expectOneCall("pwr_ll_enable_peripheral")
		.withParameter("peri", PERIPH_SPI1);
	mock().expectOneCall("clk_ll_enable_peripheral")
		.withParameter("peri", PERIPH_SPI1);
	spi_enable(PERIPH_SPI1);
}

TEST(SPI, disable_ShouldDisablePeripheralAndClock) {
	mock().expectOneCall("pwr_ll_disable_peripheral")
		.withParameter("peri", PERIPH_SPI1);
	mock().expectOneCall("clk_ll_disable_peripheral")
		.withParameter("peri", PERIPH_SPI1);
	spi_disable(PERIPH_SPI1);
}

TEST(SPI, start_ShouldStartSPI) {
	mock().expectOneCall("spi_ll_start")
		.withParameter("spi", PERIPH_SPI1);
	spi_start(PERIPH_SPI1);
}

TEST(SPI, stop_ShouldStopSPI) {
	mock().expectOneCall("spi_ll_is_busy")
		.withParameter("spi", PERIPH_SPI1)
		.andReturnValue(false);
	mock().expectOneCall("spi_ll_stop")
		.withParameter("spi", PERIPH_SPI1);
	spi_stop(PERIPH_SPI1);
}
