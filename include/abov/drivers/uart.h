#ifndef ABOV_UART_H
#define ABOV_UART_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#include "abov/hal/uart.h"

/** UART handler type */
typedef void (*uart_intr_callback_t)(uint32_t flags);

/** UART configuration */
struct uart_cfg {
	/***/
	uart_wordsize_t wordsize;
	/***/
	uart_stopbit_t stopbit;
	/***/
	uart_parity_t parity;
	/***/
	unsigned int baudrate;
	/***/
	bool rx_interrupt;
	/***/
	bool tx_interrupt;
};

/** UART handle type */
typedef union {
#if defined(__WORDSIZE) && __WORDSIZE == 64
	char _size[sizeof(struct uart_cfg) + sizeof(uart_port_t) + 24];
#else
	char _size[sizeof(struct uart_cfg) + sizeof(uart_port_t) + 12];
#endif
	long _align;
} uart_handle_t;

/**
 * Initialize UART port with given configuration
 *
 * :param handle: handle of uart port
 * :param port: a enum of :c:type:`uart_port_t`
 * :param cfg: configuration
 * :return: `true` on success
 */
bool uart_init(uart_handle_t *handle, uart_port_t port, const struct uart_cfg *cfg);
void uart_deinit(uart_handle_t *handle);

void uart_register_rx_handler(uart_handle_t *handle, uart_intr_callback_t handler);
void uart_register_tx_handler(uart_handle_t *handle, uart_intr_callback_t handler);
void uart_register_error_handler(uart_handle_t *handle, uart_intr_callback_t handler);

size_t uart_read(uart_handle_t *handle, void *buf, size_t bufsize);
size_t uart_write(uart_handle_t *handle, const void *data, size_t datasize);

void uart_default_isr(uart_port_t uartp);

#if defined(UNITTEST)
void uart_teardown(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_H */
