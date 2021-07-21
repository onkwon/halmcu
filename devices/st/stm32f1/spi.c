#include "halmcu/ll/spi.h"

#include <stddef.h>

#include "halmcu/assert.h"
#include "halmcu/bitop.h"
#include "stm32f1.h"

static SPI_Type *get_instance(periph_t spi)
{
	switch (spi) {
	case PERIPH_SPI1:
		return SPI1;
	case PERIPH_SPI2:
		return SPI2;
	case PERIPH_SPI3:
		return SPI3;
	default:
		assert(0);
		return NULL;
	}
}

static bool is_tx_busy(periph_t spi)
{
	return !(get_instance(spi)->SR & (1U << 1/*TXE*/));
}

static bool has_rx(periph_t spi)
{
	return !!(get_instance(spi)->SR & (1U << 0/*RXNE*/));
}

static void set_txd(periph_t spi, uint32_t value)
{
	get_instance(spi)->DR = value;
}

static uint32_t get_rxd(periph_t spi)
{
	return get_instance(spi)->DR;
}

static bool is_busy(periph_t spi)
{
	return !!(get_instance(spi)->SR & (1U << 7/*BSY*/));
}

void spi_ll_reset(periph_t spi)
{
	uint32_t pos = 12; /* spi1 */
	volatile uint32_t *reg = &RCC->APB2RSTR;

	if (spi == PERIPH_SPI2) {
		pos = 14;
		reg = &RCC->APB1RSTR;
	} else if (spi == PERIPH_SPI3) {
		pos = 15;
		reg = &RCC->APB1RSTR;
	}

	bitop_set(reg, pos);
	bitop_clear(reg, pos);
}

uint32_t spi_ll_get_rxd(periph_t spi)
{
	return get_rxd(spi);
}

void spi_ll_set_txd(periph_t spi, uint32_t value)
{
	set_txd(spi, value);
}

void spi_ll_write(periph_t spi, uint32_t value)
{
	while (is_tx_busy(spi)) {
		/* waiting */
	}

	set_txd(spi, value);
}

uint32_t spi_ll_read(periph_t spi)
{
	while (!has_rx(spi)) {
		/* waiting for rx */
	}

	return get_rxd(spi);
}

bool spi_ll_is_busy(periph_t spi)
{
	return is_busy(spi);
}

void spi_ll_set_clock_phase(periph_t spi, int cpha)
{
	assert(cpha == 0 || cpha == 1);
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			0/*CPHA*/, 1, (uint32_t)cpha);
}

void spi_ll_set_clock_polarity(periph_t spi, int cpol)
{
	assert(cpol == 0 || cpol == 1);
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			1/*CPOL*/, 1, (uint32_t)cpol);
}

void spi_ll_set_data_width(periph_t spi, uint32_t data_width)
{
	assert(data_width == 8 || data_width == 16);
	data_width >>= 4;
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			11/*DFF*/, 1, data_width);
}

void spi_ll_set_bitorder(periph_t spi, bool lsb_first)
{
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			7/*LSBFIRST*/, 1, lsb_first);
}

void spi_ll_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk)
{
	assert(hz <= pclk / 2);

	uint32_t div = pclk / hz;
	uint32_t bit = 0;

	while (div != 0) {
		div >>= 1;
		bit++;
	}

	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			3/*BR*/, 7, bit - 2);
}

void spi_ll_enable_irq(periph_t spi, spi_irq_t irqs)
{
	if (irqs & SPI_IRQ_RX) {
		bitop_set(&get_instance(spi)->CR2, 6); /* RXNEIE */
	}
	if (irqs & SPI_IRQ_TX) {
		bitop_set(&get_instance(spi)->CR2, 7); /* TXEIE */
	}
	if (irqs & SPI_IRQ_ERROR) {
		bitop_set(&get_instance(spi)->CR2, 5); /* ERRIE */
	}
#if 0
	if (irqs & SPI_IRQ_RX_DMA) {
		bitop_set(&get_instance(spi)->CR2, 0); /* RXDMAEN */
	}
	if (irqs & SPI_IRQ_TX_DMA) {
		bitop_set(&get_instance(spi)->CR2, 1); /* TXDMAEN */
	}
#endif
}

void spi_ll_disable_irq(periph_t spi, spi_irq_t irqs)
{
	if (irqs & SPI_IRQ_RX) {
		bitop_clear(&get_instance(spi)->CR2, 6); /* RXNEIE */
	}
	if (irqs & SPI_IRQ_TX) {
		bitop_clear(&get_instance(spi)->CR2, 7); /* TXEIE */
	}
	if (irqs & SPI_IRQ_ERROR) {
		bitop_clear(&get_instance(spi)->CR2, 5); /* ERRIE */
	}
#if 0
	if (irqs & SPI_IRQ_RX_DMA) {
		bitop_clear(&get_instance(spi)->CR2, 0); /* RXDMAEN */
	}
	if (irqs & SPI_IRQ_TX_DMA) {
		bitop_clear(&get_instance(spi)->CR2, 1); /* TXDMAEN */
	}
#endif
}

void spi_ll_enable_chip_select(periph_t spi)
{
	bitop_set(&get_instance(spi)->CR2, 2/*SSOE*/);
}

void spi_ll_disable_chip_select(periph_t spi)
{
	bitop_clear(&get_instance(spi)->CR2, 2/*SSOE*/);
}

void spi_ll_set_chip_select_mode(periph_t spi, bool manual)
{
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			9/*SSM*/, 1, manual);
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			8/*SSI*/, 1, manual);
}

void spi_ll_set_chip_select_level(periph_t spi, int level)
{
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			8/*SSI*/, 1, (uint32_t)level & 1);
}

void spi_ll_set_mode(periph_t spi, spi_mode_t mode)
{
	uint32_t val = mode == SPI_MODE_MASTER? 1 : 0;
	bitop_clean_set_with_mask(&get_instance(spi)->CR1,
			2/*MSTR*/, 1, val);
}

void spi_ll_start(periph_t spi)
{
	bitop_set(&get_instance(spi)->CR1, 6); /* SPE */
}

void spi_ll_stop(periph_t spi)
{
	bitop_clear(&get_instance(spi)->CR1, 6); /* SPE */
}

spi_event_t spi_ll_get_event(periph_t spi)
{
	uint32_t flags = get_instance(spi)->SR;
	spi_event_t events = SPI_EVENT_NONE;

	if (flags & (1U << 6)) { /* OVR */
		events = (spi_event_t)(events | SPI_EVENT_OVERRUN);
	}
	if (flags & (1U << 5)) { /* MODF */
		events = (spi_event_t)(events | SPI_EVENT_MODE_FAULT);
	}
	if (flags & (1U << 4)) { /* CRCERR */
		events = (spi_event_t)(events | SPI_EVENT_CRC_ERROR);
	}
	if (flags & (1U << 3)) { /* UDR */
		events = (spi_event_t)(events | SPI_EVENT_UNDERRUN);
	}
	if (flags & (1U << 1)) { /* TXE */
		events = (spi_event_t)(events | SPI_EVENT_TX_COMPLETE);
	}
	if (flags & (1U << 0)) { /* RXNE */
		events = (spi_event_t)(events | SPI_EVENT_RX);
	}

	return events;
}

void spi_ll_clear_event(periph_t spi, spi_event_t events)
{
	if (events & SPI_EVENT_CRC_ERROR) {
		bitop_clear(&get_instance(spi)->SR, 4); /* CRCERR */
	}
}
#if 0
void spi_ll_enable_crc(periph_t spi)
void spi_ll_disable_crc(periph_t spi)
#endif
