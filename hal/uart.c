#include "abov/hal/uart.h"

#include <string.h>

#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/irq.h"
#include "abov/compiler.h"

struct uart {
	struct uart_cfg cfg;
	uart_irq_callback_t rx_handler;
	uart_irq_callback_t tx_handler;
	uart_irq_callback_t error_handler;
};
ABOV_STATIC_ASSERT(sizeof(struct uart) == sizeof(uart_handle_t), "");

static int read_byte_nonblock(periph_t port)
{
	if (uart_has_rx(port)) {
		return uart_get_rxd(port);
	}
	return -1;
}

static void write_byte(periph_t port, uint8_t val)
{
	while (!uart_is_tx_ready(port)) {
		/* waiting */
	}
	uart_set_txd(port, (uint32_t)val);
}

bool uart_init(periph_t uart, const struct uart_cfg *cfg, uart_handle_t *handle)
{
	if (cfg == NULL) {
		return false;
	}

	pwr_enable_peripheral(uart);
	clk_enable_peripheral(uart);

	uart_reset(uart);
	uart_set_baudrate(uart, cfg->baudrate, clk_get_pclk_frequency());
	uart_set_wordsize(uart, cfg->wordsize);
	uart_set_stopbits(uart, cfg->stopbit);
	uart_set_parity(uart, cfg->parity);

	irq_t nirq = PERIPH_TO_IRQ(uart);
	if (nirq != IRQ_UNDEFINED) {
		if (cfg->rx_interrupt) {
			uart_enable_irq(uart, UART_IRQ_RX);
			irq_enable(nirq);
		}
		if (cfg->tx_interrupt) {
			uart_enable_irq(uart, UART_IRQ_TX_READY);
			irq_enable(nirq);
		}
	}

	if (handle != NULL) {
		struct uart *self = (struct uart *)handle;
		memset(self, 0, sizeof(*self));
		self->cfg = *cfg;
	}

	uart_start(uart);

	return true;
}

void uart_deinit(periph_t uart)
{
	uart_stop(uart);

	irq_disable(PERIPH_TO_IRQ(uart));

	clk_disable_peripheral(uart);
	pwr_disable_peripheral(uart);
}

int uart_read_byte_nonblock(periph_t port)
{
	return read_byte_nonblock(port);
}

int uart_read_byte(periph_t port)
{
	while (!uart_has_rx(port)) {
		/* waiting */
	}
	return uart_get_rxd(port);
}

void uart_write_byte(periph_t port, uint8_t val)
{
	write_byte(port, val);
}

size_t uart_read(periph_t uart, void *buf, size_t bufsize)
{
	uint8_t *p = (uint8_t *)buf;
	size_t received = 0;

	while (received < bufsize) {
		int val = uart_read_byte_nonblock(uart);
		if (val < 0) {
			break;
		}

		p[received++] = (uint8_t)val;
	}

	return received;
}

size_t uart_write(periph_t uart, const void *data, size_t datasize)
{
	const uint8_t *p = (const uint8_t *)data;

	for (size_t i = 0; i < datasize; i++) {
		write_byte(uart, p[i]);
	}

	return datasize;
}

void uart_register_rx_handler(uart_handle_t *handle, uart_irq_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->rx_handler = handler;
}

void uart_register_tx_handler(uart_handle_t *handle, uart_irq_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->tx_handler = handler;
}

void uart_register_error_handler(uart_handle_t *handle, uart_irq_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->error_handler = handler;
}

void uart_default_isr(periph_t uart, const uart_handle_t *handle)
{
	uart_event_t events = uart_get_event(uart);
	uart_clear_event(uart, events);

	if (handle == NULL) {
		return;
	}

	const struct uart *self = (const struct uart *)handle;

	if ((events & UART_EVENT_RX) && self->rx_handler) {
		self->rx_handler(events);
	}
	if ((events & UART_EVENT_TX_READY) && self->tx_handler) {
		self->tx_handler(events);
	}
	if ((events & UART_EVENT_ERROR) && self->error_handler) {
		self->error_handler(events);
	}
}
