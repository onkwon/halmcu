#include "abov/drivers/uart.h"
#include <string.h>
#include "abov/irq.h"
#include "abov/compiler.h"

#if !defined(UART_MAX_DRIVER_HANDLE)
#define UART_MAX_DRIVER_HANDLE			2
#endif

struct uart {
	uart_port_t port;
	struct uart_cfg cfg;
	uart_intr_callback_t rx_handler;
	uart_intr_callback_t tx_handler;
	uart_intr_callback_t error_handler;
};
ABOV_STATIC_ASSERT(sizeof(struct uart) == sizeof(uart_handle_t), "");

static struct uart *handles[UART_MAX_DRIVER_HANDLE];

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

static int get_index_of_empty_handle(void)
{
	int i;

	for (i = 0; i < UART_MAX_DRIVER_HANDLE; i++) {
		if (handles[i] == NULL) {
			return i;
		}
	}

	return -1;
}

static int get_index_of_handle_by_obj(const struct uart *obj)
{
	int i;

	for (i = 0; i < UART_MAX_DRIVER_HANDLE; i++) {
		if (handles[i] != NULL && handles[i] == obj) {
			return i;
		}
	}

	return -1;
}

static int get_index_of_handle_by_port(const uart_port_t port)
{
	int i;

	for (i = 0; i < UART_MAX_DRIVER_HANDLE; i++) {
		if (handles[i] != NULL && handles[i]->port == port) {
			return i;
		}
	}

	return -1;
}

bool uart_init(uart_handle_t *handle, uart_port_t port, const struct uart_cfg *cfg)
{
	struct uart *self = (struct uart *)handle;
	int index = get_index_of_empty_handle();

	if (index < 0) { /* no handle to allocate */
		return false;
	}
	if (self == NULL || cfg == NULL) {
		return false;
	}
	if (get_index_of_handle_by_port(port) >= 0) { /* already in use */
		return false;
	}

	memset(self, 0, sizeof(*self));
	self->port = port;
	self->cfg = *cfg;
	handles[index] = self;

	uart_reset(self->port);
	uart_set_baudrate(self->port, self->cfg.baudrate);
	uart_set_wordsize(self->port, self->cfg.wordsize);
	uart_set_stopbits(self->port, self->cfg.stopbit);
	uart_set_parity(self->port, self->cfg.parity);

	irq_t nirq = get_irq_from_port(self->port);
	if (nirq != IRQ_UNDEFINED) {
		if (self->cfg.rx_interrupt) {
			uart_enable_rx_intr(self->port);
			irq_enable(nirq);
		}
		if (self->cfg.tx_interrupt) {
			uart_enable_tx_intr(self->port);
			irq_enable(nirq);
		}
	}

	return true;
}

void uart_deinit(uart_handle_t *handle)
{
	struct uart *self = (struct uart *)handle;
	int index = get_index_of_handle_by_obj(self);
	if (index < 0) { /* not found */
		return;
	}

	handles[index] = NULL;
}

void uart_register_rx_handler(uart_handle_t *handle, uart_intr_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->rx_handler = handler;
}

void uart_register_tx_handler(uart_handle_t *handle, uart_intr_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->tx_handler = handler;
}

void uart_register_error_handler(uart_handle_t *handle, uart_intr_callback_t handler)
{
	struct uart *self = (struct uart *)handle;
	self->error_handler = handler;
}

void uart_default_isr(uart_port_t uartp)
{
	uint32_t flags = uart_get_status(uartp);
	int index = get_index_of_handle_by_port(uartp);
	uart_intr_callback_t callback = NULL;

	if (index < 0) { /* not found */
		return;
	}

	switch (flags & UART_EVENT_MASK) {
	case UART_EVENT_RX:
		callback = handles[index]->rx_handler;
		break;
	case UART_EVENT_TX_READY:
		callback = handles[index]->tx_handler;
		break;
	case UART_EVENT_ERROR:
		callback = handles[index]->error_handler;
		break;
	default:
		break;
	}

	if (callback != NULL) {
		callback(flags);
	}
}

#if defined(UNITTEST)
void uart_teardown(void)
{
	for (int i = 0; i < UART_MAX_DRIVER_HANDLE; i++) {
		handles[i] = NULL;
	}
}
#endif
