#include "abov/hal/uart.h"
#include <string.h>
#include "abov/hal/irq.h"
#include "abov/compiler.h"

struct uart {
	uart_port_t port;
	struct uart_cfg cfg;
	uart_irq_callback_t rx_handler;
	uart_irq_callback_t tx_handler;
	uart_irq_callback_t error_handler;
};
ABOV_STATIC_ASSERT(sizeof(struct uart) == sizeof(uart_handle_t), "");

static irq_t get_irq_from_port(uart_port_t port)
{
	switch (port) {
	case UART_PORT_0:
		return IRQ_UART0;
	case UART_PORT_1:
		return IRQ_UART1;
	case UART_PORT_2:
		return IRQ_UART2;
	case UART_PORT_3:
		return IRQ_UART3;
	default:
		return IRQ_UNDEFINED;
	}
}

bool uart_init(uart_handle_t *handle, uart_port_t port, const struct uart_cfg *cfg)
{
	struct uart *self = (struct uart *)handle;

	if (self == NULL || cfg == NULL) {
		return false;
	}

	memset(self, 0, sizeof(*self));
	self->port = port;
	self->cfg = *cfg;

	uart_enable(self->port);
	uart_reset(self->port);
	uart_set_baudrate(self->port, self->cfg.baudrate);
	uart_set_wordsize(self->port, self->cfg.wordsize);
	uart_set_stopbits(self->port, self->cfg.stopbit);
	uart_set_parity(self->port, self->cfg.parity);

	irq_t nirq = get_irq_from_port(self->port);
	if (nirq != IRQ_UNDEFINED) {
		if (self->cfg.rx_interrupt) {
			uart_enable_irq(self->port, UART_EVENT_RX);
			irq_enable(nirq);
		}
		if (self->cfg.tx_interrupt) {
			uart_enable_irq(self->port, UART_EVENT_TX_READY);
			irq_enable(nirq);
		}
	}

	return true;
}

void uart_deinit(uart_handle_t *handle)
{
	const struct uart *self = (const struct uart *)handle;
	if (self == NULL) {
		return;
	}

	irq_disable(get_irq_from_port(self->port));
	uart_disable(self->port);
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

size_t uart_read(uart_handle_t *handle, void *buf, size_t bufsize)
{
	const struct uart *self = (const struct uart *)handle;
	uint8_t *p = (uint8_t *)buf;
	size_t received = 0;

	while (received < bufsize) {
		int val = uart_read_byte_nonblock(self->port);
		if (val < 0) {
			break;
		}

		p[received++] = (uint8_t)val;
	}

	return received;
}

size_t uart_write(uart_handle_t *handle, const void *data, size_t datasize)
{
	const struct uart *self = (const struct uart *)handle;
	const uint8_t *p = (const uint8_t *)data;

	for (size_t i = 0; i < datasize; i++) {
		uart_write_byte(self->port, p[i]);
	}

	return datasize;
}

void uart_default_isr(uart_port_t uartp, uart_handle_t *handle)
{
	uart_event_t events = uart_get_event(uartp);
	uart_clear_event(uartp, events);

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
