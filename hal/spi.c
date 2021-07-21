#include "halmcu/hal/spi.h"

#include <stddef.h>

#include "halmcu/ll/pwr.h"
#include "halmcu/periph/clk.h"
#include "halmcu/irq.h"

void spi_enable(periph_t spi)
{
	pwr_ll_enable_peripheral(spi);
	clk_enable_peripheral(spi);
}

void spi_disable(periph_t spi)
{
	clk_disable_peripheral(spi);
	pwr_ll_disable_peripheral(spi);
}

void spi_start(periph_t spi)
{
	spi_ll_start(spi);
}

void spi_stop(periph_t spi)
{
	while (spi_ll_is_busy(spi)) {
		/* waiting for ongoing job finished */
	}

	spi_ll_stop(spi);
}

bool spi_init(periph_t spi, const struct spi_cfg *cfg)
{
	if (cfg == NULL || cfg->frequency == 0) {
		return false;
	}

	spi_enable(spi);
	spi_ll_reset(spi);
	spi_ll_set_mode(spi, cfg->mode);
	spi_ll_set_frequency(spi, cfg->frequency,
			clk_get_peripheral_clock_source_frequency(spi));
	spi_ll_set_clock_polarity(spi, cfg->cpol);
	spi_ll_set_clock_phase(spi, cfg->cpha);
	spi_ll_set_data_width(spi, cfg->data_width);
	spi_ll_set_bitorder(spi, cfg->lsb_first);
	spi_ll_set_chip_select_mode(spi, !cfg->auto_chip_select);

	spi_ll_disable_chip_select(spi);
	if (cfg->auto_chip_select) {
		spi_ll_enable_chip_select(spi);
	}

	if (cfg->interrupt) {
		spi_ll_enable_irq(spi, (spi_irq_t)
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
	spi_ll_write(spi, value);
}

uint32_t spi_read(periph_t spi)
{
	return spi_ll_read(spi);
}

uint32_t spi_write_read(periph_t spi, uint32_t value)
{
	spi_ll_write(spi, value);
	return spi_ll_read(spi);
}

spi_event_t spi_get_event(periph_t spi)
{
	return spi_ll_get_event(spi);
}

void spi_clear_event(periph_t spi, spi_event_t events)
{
	spi_ll_clear_event(spi, events);
}
