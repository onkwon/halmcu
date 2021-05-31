#ifndef ABOV_SPI_LL_H
#define ABOV_SPI_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/periph/spi.h"

/**
 * Reset SPI
 *
 * This function makes the given SPI the reset default state.
 *
 * @param[in] spi a peripheral enumerated in @ref periph_t
 */
void spi_ll_reset(periph_t spi);
uint32_t spi_ll_get_rxd(periph_t spi);
void spi_ll_set_txd(periph_t spi, uint32_t value);
void spi_ll_write(periph_t spi, uint32_t value);
uint32_t spi_ll_read(periph_t spi);
bool spi_ll_is_busy(periph_t spi);
void spi_ll_clear_rx_buffer(periph_t spi);
void spi_ll_clear_tx_buffer(periph_t spi);
void spi_ll_enable_irq(periph_t spi, spi_irq_t irqs);
void spi_ll_disable_irq(periph_t spi, spi_irq_t irqs);
void spi_ll_enable_chip_select(periph_t spi);
void spi_ll_disable_chip_select(periph_t spi);
void spi_ll_set_chip_select_mode(periph_t spi, bool manual);
void spi_ll_set_chip_select_level(periph_t spi, int level);
void spi_ll_set_chip_select_polarity(periph_t spi, int level);
void spi_ll_set_loopback(periph_t spi, bool enable);
void spi_ll_set_mode(periph_t spi, spi_mode_t mode);
void spi_ll_start(periph_t spi);
void spi_ll_stop(periph_t spi);
spi_event_t spi_ll_get_event(periph_t spi);
void spi_ll_clear_event(periph_t spi, spi_event_t events);
/**
 * Set clock phase
 *
 * @param[in] spi a peripheral enumerated in @ref periph_t
 * @param[in] cpha 0 for the first clock edge or 1 for the second clock edge
 */
void spi_ll_set_clock_phase(periph_t spi, int cpha);
/**
 * Set clock polarity
 *
 * @param[in] spi a peripheral enumerated in @ref periph_t
 * @param[in] cpol 0 for low, 1 for high
 */
void spi_ll_set_clock_polarity(periph_t spi, int cpol);
void spi_ll_set_data_width(periph_t spi, uint32_t data_width);
void spi_ll_set_bitorder(periph_t spi, bool lsb_first);
void spi_ll_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk);
void spi_ll_set_start_delay(periph_t spi, uint32_t nsck);
void spi_ll_set_stop_delay(periph_t spi, uint32_t nsck);
void spi_ll_set_burst_delay(periph_t spi, uint32_t nsck);

void spi_ll_enable_crc(periph_t spi);
void spi_ll_disable_crc(periph_t spi);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SPI_LL_H */
