#ifndef ABOV_SPI_HAL_H
#define ABOV_SPI_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/spi.h"

void spi_enable(periph_t spi);
void spi_disable(periph_t spi);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SPI_HAL_H */
