#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "halmcu/periph/spi.h"
#include "stm32f1.h"

extern "C" {
static SPI_Type spi1_reg, spi2_reg, spi3_reg;
static RCC_Type rcc_reg;
SPI_Type * const SPI1 = &spi1_reg;
SPI_Type * const SPI2 = &spi2_reg;
SPI_Type * const SPI3 = &spi3_reg;
RCC_Type * const RCC = &rcc_reg;
}

TEST_GROUP(SPI) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(SPI1, 0, sizeof(*SPI1));
		memset(SPI2, 0, sizeof(*SPI2));
		memset(SPI3, 0, sizeof(*SPI3));
		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(SPI, reset_ShouldSetAndClearRSTR) {
	mock().expectOneCall("bitop_set").withParameter("pos", 12);
	mock().expectOneCall("bitop_clear").withParameter("pos", 12);
	spi_reset(PERIPH_SPI1);

	mock().expectOneCall("bitop_set").withParameter("pos", 14);
	mock().expectOneCall("bitop_clear").withParameter("pos", 14);
	spi_reset(PERIPH_SPI2);

	mock().expectOneCall("bitop_set").withParameter("pos", 15);
	mock().expectOneCall("bitop_clear").withParameter("pos", 15);
	spi_reset(PERIPH_SPI3);
}

TEST(SPI, get_rxd_ShouldReturnReceivedData) {
	SPI1->DR = 0xa5;
	LONGS_EQUAL(0xa5, spi_get_rxd(PERIPH_SPI1));
}

TEST(SPI, set_txd_ShouldSetTransmitData) {
	spi_set_txd(PERIPH_SPI1, 0x5a);
	LONGS_EQUAL(0x5a, SPI1->DR);
}

TEST(SPI, is_busy_ShouldReturnCurrentState) {
	SPI1->SR = 0x80; // BSY
	LONGS_EQUAL(1, spi_is_busy(PERIPH_SPI1));
}

TEST(SPI, set_clock_phase_ShouldSetCPHA) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 0)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_clock_phase(PERIPH_SPI1, 1);
}

TEST(SPI, set_clock_phase_ShouldSetCPOL) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 1)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_clock_polarity(PERIPH_SPI1, 1);
}

TEST(SPI, set_data_width_ShouldSetDFF_WhenDatawidthIs8) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 11)
		.withParameter("val", 0)
		.ignoreOtherParameters();
	spi_set_data_width(PERIPH_SPI1, 8);
}
TEST(SPI, set_data_width_ShouldSetDFF_WhenDatawidthIs16) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 11)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_data_width(PERIPH_SPI1, 16);
}

TEST(SPI, set_bitorder_ShouldSetLSBFIRST) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 7)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_bitorder(PERIPH_SPI1, true);
}
TEST(SPI, set_bitorder_ShouldClerLSBFIRST_WhenMSBFirst) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 7)
		.withParameter("val", 0)
		.ignoreOtherParameters();
	spi_set_bitorder(PERIPH_SPI1, false);
}

TEST(SPI, set_frequency_ShouldSetBR_WhenMaxFreqGiven) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 3)
		.withParameter("val", 0)
		.ignoreOtherParameters();
	spi_set_frequency(PERIPH_SPI1, 500000, 1000000);
}
TEST(SPI, set_frequency_ShouldSetBR_WhenFourthFreqGiven) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 3)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_frequency(PERIPH_SPI1, 250000, 1000000);
}
TEST(SPI, set_frequency_ShouldSetBR_When256thFreqGiven) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 3)
		.withParameter("val", 7)
		.ignoreOtherParameters();
	spi_set_frequency(PERIPH_SPI1, 31250, 8000000);
}

TEST(SPI, enable_irq_ShouldEnableIRQ) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 6)
		.ignoreOtherParameters();
	spi_enable_irq(PERIPH_SPI1, SPI_IRQ_RX);
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 7)
		.ignoreOtherParameters();
	spi_enable_irq(PERIPH_SPI1, SPI_IRQ_TX);
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 5)
		.ignoreOtherParameters();
	spi_enable_irq(PERIPH_SPI1, SPI_IRQ_ERROR);
}
TEST(SPI, disable_irq_ShouldDisableIRQ) {
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 6)
		.ignoreOtherParameters();
	spi_disable_irq(PERIPH_SPI1, SPI_IRQ_RX);
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 7)
		.ignoreOtherParameters();
	spi_disable_irq(PERIPH_SPI1, SPI_IRQ_TX);
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 5)
		.ignoreOtherParameters();
	spi_disable_irq(PERIPH_SPI1, SPI_IRQ_ERROR);
}

TEST(SPI, enable_chip_select_ShouldSetSSOE) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 2)
		.ignoreOtherParameters();
	spi_enable_chip_select(PERIPH_SPI1);
}

TEST(SPI, disable_chip_select_ShouldClearSSOE) {
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 2)
		.ignoreOtherParameters();
	spi_disable_chip_select(PERIPH_SPI1);
}

TEST(SPI, set_chip_select_mode_ShouldSetSSMAndSSI) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 9)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 8)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_chip_select_mode(PERIPH_SPI1, true);
}

TEST(SPI, set_chip_select_level_ShouldSetSSI) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 8)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_chip_select_level(PERIPH_SPI1, 1);
}

TEST(SPI, set_mode_ShouldSetMSTR_WhenMaster) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 2)
		.withParameter("val", 1)
		.ignoreOtherParameters();
	spi_set_mode(PERIPH_SPI1, SPI_MODE_MASTER);
}
TEST(SPI, set_mode_ShouldClearMSTR_WhenSlave) {
	mock().expectOneCall("bitop_clean_set_with_mask")
		.withParameter("pos", 2)
		.withParameter("val", 0)
		.ignoreOtherParameters();
	spi_set_mode(PERIPH_SPI1, SPI_MODE_SLAVE);
}

TEST(SPI, start_ShouldSetSPE) {
	mock().expectOneCall("bitop_set")
		.withParameter("pos", 6)
		.ignoreOtherParameters();
	spi_enable_clock(PERIPH_SPI1);
}
TEST(SPI, stop_ShouldClearSPE) {
	mock().expectOneCall("bitop_clear")
		.withParameter("pos", 6)
		.ignoreOtherParameters();
	spi_disable_clock(PERIPH_SPI1);
}

TEST(SPI, get_event_ShouldReturnEvents) {
	SPI1->SR = 0xff;
	LONGS_EQUAL(SPI_EVENT_OVERRUN | SPI_EVENT_MODE_FAULT |
			SPI_EVENT_CRC_ERROR | SPI_EVENT_UNDERRUN |
			SPI_EVENT_TX_COMPLETE | SPI_EVENT_RX,
			spi_get_event(PERIPH_SPI1));
}

TEST(SPI, clear_event_ShouldClearCrcErr) {
	SPI1->SR = 0x10; // CRCERR
	spi_clear_event(PERIPH_SPI1, SPI_EVENT_CRC_ERROR);
	LONGS_EQUAL(0, SPI1->SR);
}
