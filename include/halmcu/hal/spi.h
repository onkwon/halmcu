#ifndef HALMCU_SPI_HAL_H
#define HALMCU_SPI_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/ll/spi.h"

struct spi_cfg {
	spi_mode_t mode;
	uint32_t frequency;
	int cpol;
	int cpha;
	uint32_t data_width;
	bool lsb_first; /* msb_first if false */
	bool auto_chip_select;
	bool interrupt;
};

void spi_enable(periph_t spi);
void spi_disable(periph_t spi);
bool spi_init(periph_t spi, const struct spi_cfg *cfg);
void spi_deinit(periph_t spi);
void spi_write(periph_t spi, uint32_t value);
uint32_t spi_read(periph_t spi);
uint32_t spi_write_read(periph_t spi, uint32_t value);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_SPI_HAL_H */
