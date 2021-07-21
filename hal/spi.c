#include "halmcu/hal/spi.h"

#include <stddef.h>

#include "halmcu/periph/pwr.h"
#include "halmcu/periph/clk.h"
#include "halmcu/irq.h"

static void spi_write_internal(periph_t spi, uint32_t value)
{
	while (!spi_is_tx_completed(spi)) {
		/* waiting */
	}
	spi_set_txd(spi, value);
}

static uint32_t spi_read_internal(periph_t spi)
{
	while (!spi_has_rx(spi)) {
		/* waiting for rx */
	}
	return spi_get_rxd(spi);
}

void spi_enable(periph_t spi)
{
	pwr_enable_peripheral(spi);
	clk_enable_peripheral(spi);
}

void spi_disable(periph_t spi)
{
	clk_disable_peripheral(spi);
	pwr_disable_peripheral(spi);
}

void spi_start(periph_t spi)
{
	spi_enable_clock(spi);
}

void spi_stop(periph_t spi)
{
	while (spi_is_busy(spi)) {
		/* waiting for ongoing job finished */
	}

	spi_disable_clock(spi);
}

bool spi_init(periph_t spi, const struct spi_cfg *cfg)
{
	if (cfg == NULL || cfg->frequency == 0) {
		return false;
	}

	spi_enable(spi);
	spi_reset(spi);
	spi_set_mode(spi, cfg->mode);
	spi_set_frequency(spi, cfg->frequency,
			clk_get_peripheral_clock_source_frequency(spi));
	spi_set_clock_polarity(spi, cfg->cpol);
	spi_set_clock_phase(spi, cfg->cpha);
	spi_set_data_width(spi, cfg->data_width);
	spi_set_bitorder(spi, cfg->lsb_first);
	spi_set_chip_select_mode(spi, !cfg->auto_chip_select);

	spi_disable_chip_select(spi);
	if (cfg->auto_chip_select) {
		spi_enable_chip_select(spi);
	}

	if (cfg->interrupt) {
		spi_enable_irq(spi, (spi_irq_t)
			(/*SPI_IRQ_RX |*/ /*SPI_IRQ_TX |*/ SPI_IRQ_ERROR));
		irq_enable(PERIPH_TO_IRQ(spi));
	}

	return true;
}

void spi_deinit(periph_t spi)
{
	irq_disable(PERIPH_TO_IRQ(spi));
	spi_disable(spi);
}

void spi_write(periph_t spi, uint32_t value)
{
	spi_write_internal(spi, value);
}

uint32_t spi_read(periph_t spi)
{
	return spi_read_internal(spi);
}

uint32_t spi_write_read(periph_t spi, uint32_t value)
{
	spi_write_internal(spi, value);
	return spi_read_internal(spi);
}
