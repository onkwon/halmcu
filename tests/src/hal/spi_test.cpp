#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/hal/spi.h"

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

TEST(SPI, init_ShouldReturnFalse_WhenInvalidParamGiven) {
	LONGS_EQUAL(0, spi_init(PERIPH_SPI1, NULL));
}
TEST(SPI, init_ShouldReturnTrue) {
	struct spi_cfg cfg = {
		.frequency = 100000,
	};
	LONGS_EQUAL(1, spi_init(PERIPH_SPI1, &cfg));
}
TEST(SPI, init_ShouldEnableAutoChipSelect) {
	struct spi_cfg cfg = {
		.frequency = 100000,
		.auto_chip_select = true,
	};
	mock().expectOneCall("spi_ll_enable_chip_select")
		.withParameter("spi", PERIPH_SPI1);

	spi_init(PERIPH_SPI1, &cfg);
}
TEST(SPI, init_ShouldEnableInterrupt) {
	struct spi_cfg cfg = {
		.frequency = 100000,
		.interrupt = true,
	};
	mock().expectOneCall("spi_ll_enable_irq")
		.withParameter("spi", PERIPH_SPI1)
		.ignoreOtherParameters();
	mock().expectOneCall("irq_enable")
		.ignoreOtherParameters();

	spi_init(PERIPH_SPI1, &cfg);
}

TEST(SPI, deinit_ShouldDisableSPI) {
	mock().expectOneCall("irq_disable")
		.ignoreOtherParameters();
	mock().expectOneCall("pwr_ll_disable_peripheral")
		.ignoreOtherParameters();
	mock().expectOneCall("clk_ll_disable_peripheral")
		.ignoreOtherParameters();

	spi_deinit(PERIPH_SPI1);
}

TEST(SPI, write_ShouldCallLowLevelWrite) {
	mock().expectOneCall("spi_ll_write")
		.withParameter("spi", PERIPH_SPI1)
		.withParameter("value", 0xa5);
	spi_write(PERIPH_SPI1, 0xa5);
}
TEST(SPI, read_ShouldCallLowLevelRead) {
	mock().expectOneCall("spi_ll_read")
		.withParameter("spi", PERIPH_SPI1);
	spi_read(PERIPH_SPI1);
}
TEST(SPI, write_read_ShouldCallLowLevelWriteAndRead) {
	mock().expectOneCall("spi_ll_write")
		.withParameter("spi", PERIPH_SPI1)
		.withParameter("value", 0xa5);
	mock().expectOneCall("spi_ll_read")
		.withParameter("spi", PERIPH_SPI1);
	spi_write_read(PERIPH_SPI1, 0xa5);
}

TEST(SPI, get_event_ShouldCallLowLevelDriver) {
	mock().expectOneCall("spi_ll_get_event")
		.withParameter("spi", PERIPH_SPI1);
	spi_get_event(PERIPH_SPI1);
}
TEST(SPI, clear_event_ShouldCallLowLevelDriver) {
	mock().expectOneCall("spi_ll_clear_event")
		.withParameter("spi", PERIPH_SPI1)
		.withParameter("events", SPI_EVENT_RX | SPI_EVENT_OVERRUN);
	spi_clear_event(PERIPH_SPI1, (spi_event_t)
			(SPI_EVENT_RX | SPI_EVENT_OVERRUN));
}
