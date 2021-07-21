#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/spi.h"
#include "a33g.h"

extern "C" {
static struct spi_stub spireg0, spireg1;
struct spi_stub * const SPI0 = &spireg0;
struct spi_stub * const SPI1 = &spireg1;
}

TEST_GROUP(SPI) {
	void setup(void) {
		mock().ignoreOtherCalls();

		spi_ll_reset(PERIPH_SPI0);
		spi_ll_reset(PERIPH_SPI1);
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(SPI, reset_ShouldMakeInterfaceDefaultState) {
	LONGS_EQUAL(0, SPI0->TDR);
	LONGS_EQUAL(0x820, SPI0->CR);
	LONGS_EQUAL(0, SPI0->SR);
	LONGS_EQUAL(0xff, SPI0->BR);
	LONGS_EQUAL(0, SPI0->EN);
	LONGS_EQUAL(0x1866, SPI0->LR);
}

TEST(SPI, get_rxd_ShouldReturnRDR) {
	uint32_t fixed_value = 0xa5a5a5a5;
	SPI0->RDR = fixed_value;
	LONGS_EQUAL(fixed_value, SPI0->RDR);
}

TEST(SPI, set_txd_ShouldSetTDR) {
	uint32_t expected = 0x5a5a5a;
	spi_ll_set_txd(PERIPH_SPI0, expected);
	LONGS_EQUAL(expected, SPI0->TDR);
}

TEST(SPI, enable_chip_select_ShouldSetSSMO) {
	spi_ll_enable_chip_select(PERIPH_SPI0);
	LONGS_EQUAL(0x100, SPI0->CR & 0x100);
}

TEST(SPI, disable_chip_select_ShouldClearSSMO) {
	spi_ll_enable_chip_select(PERIPH_SPI0);
	spi_ll_disable_chip_select(PERIPH_SPI0);
	LONGS_EQUAL(0, SPI0->CR & 0x100);
}

TEST(SPI, set_chip_select_mode_ShouldSetSSMODE) {
	spi_ll_set_chip_select_mode(PERIPH_SPI0, true);
	LONGS_EQUAL(0x1000, SPI0->CR & 0x1000);
	spi_ll_set_chip_select_mode(PERIPH_SPI0, false);
	LONGS_EQUAL(0, SPI0->CR & 0x1000);
}

TEST(SPI, set_chip_select_level_ShouldSetSSOUT) {
	spi_ll_set_chip_select_level(PERIPH_SPI0, 1);
	LONGS_EQUAL(0x800, SPI0->CR & 0x800);
	spi_ll_set_chip_select_level(PERIPH_SPI0, 0);
	LONGS_EQUAL(0, SPI0->CR & 0x800);
}

TEST(SPI, set_chip_select_polarity_ShouldSetSSPOL) {
	spi_ll_set_chip_select_polarity(PERIPH_SPI0, 1);
	LONGS_EQUAL(0x80, SPI0->CR & 0x80);
	spi_ll_set_chip_select_polarity(PERIPH_SPI0, 0);
	LONGS_EQUAL(0, SPI0->CR & 0x80);
}

TEST(SPI, set_loopback_ShouldSetLBE) {
	spi_ll_set_loopback(PERIPH_SPI0, true);
	LONGS_EQUAL(0x400, SPI0->CR & 0x400);
	spi_ll_set_loopback(PERIPH_SPI0, false);
	LONGS_EQUAL(0, SPI0->CR & 0x400);
}

TEST(SPI, set_mode_ShouldSetMS) {
	spi_ll_set_mode(PERIPH_SPI0, SPI_MODE_MASTER);
	LONGS_EQUAL(0x20, SPI0->CR & 0x20);
	spi_ll_set_mode(PERIPH_SPI0, SPI_MODE_SLAVE);
	LONGS_EQUAL(0, SPI0->CR & 0x20);
}

TEST(SPI, set_clock_phase_ShouldSetCPHA) {
	spi_ll_set_clock_phase(PERIPH_SPI0, 1);
	LONGS_EQUAL(0x8, SPI0->CR & 0x8);
	spi_ll_set_clock_phase(PERIPH_SPI0, 0);
	LONGS_EQUAL(0, SPI0->CR & 0x8);
}

TEST(SPI, set_clock_polarity_ShouldCPOL) {
	spi_ll_set_clock_polarity(PERIPH_SPI0, 1);
	LONGS_EQUAL(0x4, SPI0->CR & 0x4);
	spi_ll_set_clock_polarity(PERIPH_SPI0, 0);
	LONGS_EQUAL(0, SPI0->CR & 0x4);
}

TEST(SPI, set_data_width_ShouldSetBITSEL) {
	spi_ll_set_data_width(PERIPH_SPI0, 17);
	LONGS_EQUAL(3, SPI0->CR & 0x3);
	spi_ll_set_data_width(PERIPH_SPI0, 16);
	LONGS_EQUAL(2, SPI0->CR & 0x3);
	spi_ll_set_data_width(PERIPH_SPI0, 9);
	LONGS_EQUAL(1, SPI0->CR & 0x3);
	spi_ll_set_data_width(PERIPH_SPI0, 8);
	LONGS_EQUAL(0, SPI0->CR & 0x3);
}

TEST(SPI, set_bitorder_ShouldSetMSBF) {
	spi_ll_set_bitorder(PERIPH_SPI0, false);
	LONGS_EQUAL(0x10, SPI0->CR & 0x10);
	spi_ll_set_bitorder(PERIPH_SPI0, true);
	LONGS_EQUAL(0, SPI0->CR & 0x10);
}

TEST(SPI, enable_irq_ShouldEnableInterrupts) {
	spi_ll_enable_irq(PERIPH_SPI0, SPI_IRQ_RX);
	LONGS_EQUAL(0x2000, SPI0->CR & 0xE000);
	spi_ll_enable_irq(PERIPH_SPI0, SPI_IRQ_TX);
	LONGS_EQUAL(0x6000, SPI0->CR & 0xE000);
	spi_ll_enable_irq(PERIPH_SPI0, SPI_IRQ_EDGE_CHAGNE);
	LONGS_EQUAL(0xE000, SPI0->CR & 0xE000);
}

TEST(SPI, disable_irq_ShouldDisableInterrupts) {
	spi_ll_enable_irq(PERIPH_SPI0, (spi_irq_t)(SPI_IRQ_RX | SPI_IRQ_TX | SPI_IRQ_EDGE_CHAGNE));
	spi_ll_disable_irq(PERIPH_SPI0, SPI_IRQ_EDGE_CHAGNE);
	LONGS_EQUAL(0x6000, SPI0->CR & 0xE000);
	spi_ll_disable_irq(PERIPH_SPI0, SPI_IRQ_TX);
	LONGS_EQUAL(0x2000, SPI0->CR & 0xE000);
	spi_ll_disable_irq(PERIPH_SPI0, SPI_IRQ_RX);
	LONGS_EQUAL(0, SPI0->CR & 0xE000);
}

TEST(SPI, set_frequency_ShouldSetBR) {
	spi_ll_set_frequency(PERIPH_SPI0, 1000000, 16000000);
	LONGS_EQUAL(16-1, SPI0->BR);
}

TEST(SPI, start_ShouldSetEN) {
	spi_ll_start(PERIPH_SPI0);
	LONGS_EQUAL(1, SPI0->EN);
}

TEST(SPI, stop_ShouldClearEN) {
	spi_ll_start(PERIPH_SPI0);
	spi_ll_stop(PERIPH_SPI0);
	LONGS_EQUAL(0, SPI0->EN);
}

TEST(SPI, get_event_ShouldReturnSR) {
	SPI0->SR = 0x7f;
	LONGS_EQUAL(0x7f, spi_ll_get_event(PERIPH_SPI0));
}

TEST(SPI, clear_event_ShouldClearSR) {
	SPI0->SR = 0x7f;
	spi_ll_clear_event(PERIPH_SPI0, SPI_EVENT_OVERRUN);
	LONGS_EQUAL(0x6f, SPI0->SR);
	spi_ll_clear_event(PERIPH_SPI0, SPI_EVENT_UNDERRUN);
	LONGS_EQUAL(0x67, SPI0->SR);
	spi_ll_clear_event(PERIPH_SPI0, SPI_EVENT_CHIP_DESELECTED);
	LONGS_EQUAL(0x27, SPI0->SR);
}
