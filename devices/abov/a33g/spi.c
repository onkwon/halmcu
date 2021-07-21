#include "halmcu/ll/spi.h"

#include <stddef.h>

#include "halmcu/assert.h"
#include "halmcu/compiler.h"
#include "halmcu/bitop.h"
#include "halmcu/asm/arm/cmsis.h"
#include "a33g.h"

static SPI_Type *get_interface_from_type(periph_t peri)
{
	SPI_Type *spi = NULL;

	switch (peri) {
	case PERIPH_SPI0:
		spi = SPI0;
		break;
	case PERIPH_SPI1:
		spi = SPI1;
		break;
	default:
		break;
	}

	assert(spi != NULL);

	return spi;
}

void spi_ll_reset(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);

	unused(self->RDR); /* read to clear RRDY in SR register */
	self->TDR = 0;
	self->CR = 0x820;
	self->SR = 0;
	self->BR = 0xff;
	self->EN = 0;
	self->LR = 0x1866;
}

uint32_t spi_ll_get_rxd(periph_t spi)
{
	const SPI_Type *self = get_interface_from_type(spi);
	return self->RDR;
}

void spi_ll_set_txd(periph_t spi, uint32_t value)
{
	SPI_Type *self = get_interface_from_type(spi);
	self->TDR = value;
}

void spi_ll_clear_rx_buffer(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_set(&self->CR, 19); /* RXBC */
}

void spi_ll_clear_tx_buffer(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_set(&self->CR, 20); /* TXBC */
}

void spi_ll_enable_chip_select(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_set(&self->CR, 8); /* SSMO */
}

void spi_ll_disable_chip_select(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_clear(&self->CR, 8); /* SSMO */
}

void spi_ll_set_chip_select_mode(periph_t spi, bool manual)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (manual) {
		bitop_set(&self->CR, 12); /* SSMODE */
	} else {
		bitop_clear(&self->CR, 12); /* SSMODE */
	}
}

void spi_ll_set_chip_select_level(periph_t spi, int level)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (level == 0) {
		bitop_clear(&self->CR, 11); /* SSOUT */
	} else {
		bitop_set(&self->CR, 11); /* SSOUT */
	}
}

void spi_ll_set_chip_select_polarity(periph_t spi, int level)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (level == 0) {
		bitop_clear(&self->CR, 7); /* SSPOL */
	} else {
		bitop_set(&self->CR, 7); /* SSPOL */
	}
}

void spi_ll_set_loopback(periph_t spi, bool enable)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (enable) {
		bitop_set(&self->CR, 10); /* LBE */
	} else {
		bitop_clear(&self->CR, 10); /* LBE */
	}
}

void spi_ll_set_mode(periph_t spi, spi_mode_t mode)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (mode == SPI_MODE_MASTER) {
		bitop_set(&self->CR, 5); /* MS */
	} else if (mode == SPI_MODE_SLAVE) {
		bitop_clear(&self->CR, 5); /* MS */
	}
}

void spi_ll_set_clock_phase(periph_t spi, int cpha)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (cpha == 1) {
		bitop_set(&self->CR, 3); /* CPHA */
	} else if (cpha == 0) {
		bitop_clear(&self->CR, 3); /* CPHA */
	}
}

void spi_ll_set_clock_polarity(periph_t spi, int cpol)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (cpol == 1) {
		bitop_set(&self->CR, 2); /* CPOL */
	} else if (cpol == 0) {
		bitop_clear(&self->CR, 2); /* CPOL */
	}
}

void spi_ll_set_data_width(periph_t spi, uint32_t data_width)
{
	SPI_Type *self = get_interface_from_type(spi);

	uint32_t val = 0;
	switch (data_width) {
	case 8:
		val = 0;
		break;
	case 9:
		val = 1;
		break;
	case 16:
		val = 2;
		break;
	case 17:
		val = 3;
		break;
	default:
		break;
	}

	bitop_clean_set_with_mask(&self->CR, 0, 0x3, val);
}

void spi_ll_set_bitorder(periph_t spi, bool lsb_first)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_clean_set_with_mask(&self->CR, 4, 1U, !lsb_first); /* MSBF */
}

void spi_ll_enable_irq(periph_t spi, spi_irq_t irqs)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (irqs & SPI_IRQ_RX) {
		bitop_set(&self->CR, 13); /* RXIE */
	}
	if (irqs & SPI_IRQ_TX) {
		bitop_set(&self->CR, 14); /* TXIE */
	}
	if (irqs & SPI_IRQ_EDGE_CHAGNE) {
		bitop_set(&self->CR, 15); /* SSCIE */
	}
}

void spi_ll_disable_irq(periph_t spi, spi_irq_t irqs)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (irqs & SPI_IRQ_RX) {
		bitop_clear(&self->CR, 13); /* RXIE */
	}
	if (irqs & SPI_IRQ_TX) {
		bitop_clear(&self->CR, 14); /* TXIE */
	}
	if (irqs & SPI_IRQ_EDGE_CHAGNE) {
		bitop_clear(&self->CR, 15); /* SSCIE */
	}
}

void spi_ll_set_frequency(periph_t spi, uint32_t hz, uint32_t pclk)
{
	SPI_Type *self = get_interface_from_type(spi);
	assert(pclk > hz);
	uint32_t brr = pclk / hz - 1;
	assert(brr > 0);
	self->BR = brr;
}

void spi_ll_start(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_set(&self->EN, 0);
}

void spi_ll_stop(periph_t spi)
{
	SPI_Type *self = get_interface_from_type(spi);
	bitop_clear(&self->EN, 0);
}

spi_event_t spi_ll_get_event(periph_t spi)
{
	const SPI_Type *self = get_interface_from_type(spi);
	spi_event_t events = SPI_EVENT_NONE;
	uint32_t flags = self->SR;

	if (flags & 1) { /* RRDY */
		events = (spi_event_t)(events | SPI_EVENT_RX);
	}
	if (flags & 2) { /* TRDY */
		events = (spi_event_t)(events | SPI_EVENT_TX_COMPLETE);
	}
	if (flags & 4) { /* SBUSY */
		events = (spi_event_t)(events | SPI_EVENT_BUSY);
	}
	if (flags & 8) { /* UDRF */
		events = (spi_event_t)(events | SPI_EVENT_UNDERRUN);
	}
	if (flags & 0x10) { /* OVRF */
		events = (spi_event_t)(events | SPI_EVENT_OVERRUN);
	}
	if (flags & 0x20) { /* SSON */
		events = (spi_event_t)(events | SPI_EVENT_CHIP_SELECTED);
	}
	if (flags & 0x40) { /* SSDET */
		events = (spi_event_t)(events | SPI_EVENT_CHIP_DESELECTED);
	}

	return events;
}

void spi_ll_clear_event(periph_t spi, spi_event_t events)
{
	SPI_Type *self = get_interface_from_type(spi);

	if (events & SPI_EVENT_CHIP_DESELECTED) {
		bitop_clear(&self->SR, 6);
	}
	if (events & SPI_EVENT_OVERRUN) {
		bitop_clear(&self->SR, 4);
	}
	if (events & SPI_EVENT_UNDERRUN) {
		bitop_clear(&self->SR, 3);
	}
}
