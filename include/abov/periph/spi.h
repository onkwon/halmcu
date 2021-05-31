#ifndef ABOV_SPI_H
#define ABOV_SPI_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/periph/periph.h"

typedef enum {
	SPI_MODE_MASTER,
	SPI_MODE_SLAVE,
} spi_mode_t;

typedef enum {
	SPI_IRQ_NONE				= 0,
	SPI_IRQ_RX				= (1U << 0),
	SPI_IRQ_TX				= (1U << 1),
	SPI_IRQ_EDGE_CHAGNE			= (1U << 2),
	SPI_IRQ_OVERRUN				= (1U << 3),
	SPI_IRQ_FRAME_ERROR			= (1U << 4),
	SPI_IRQ_ERROR				= (1U << 5),
	SPI_IRQ_MASK				= 0x3F,
} spi_irq_t;

typedef enum {
	SPI_EVENT_NONE				= 0,
	SPI_EVENT_TX_COMPLETE			= (1U << 0),
	SPI_EVENT_RX				= (1U << 1),
	SPI_EVENT_BUSY				= (1U << 2),
	SPI_EVENT_OVERRUN			= (1U << 3),
	SPI_EVENT_UNDERRUN			= (1U << 4),
	SPI_EVENT_CHIP_SELECTED			= (1U << 5),
	SPI_EVENT_CHIP_DESELECTED		= (1U << 6),
	SPI_EVENT_MASK				= 0x7F,
} spi_event_t;

void spi_set_mode(periph_t spi, spi_mode_t mode);
void spi_start(periph_t spi);
void spi_stop(periph_t spi);
void spi_set_loopback(periph_t spi, bool enable);
spi_event_t spi_get_event(periph_t spi);
void spi_clear_event(periph_t spi, spi_event_t events);
void spi_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_SPI_H */
