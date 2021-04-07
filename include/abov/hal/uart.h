#ifndef ABOV_UART_HAL_H
#define ABOV_UART_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>

#include "abov/ll/uart.h"

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
/**
 * Deinitialize UART port
 *
 * :param handle: handle of uart port
 */
void uart_deinit(uart_handle_t *handle);
/**
 * Register rx interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: rx interrupt handler
 */
void uart_register_rx_handler(uart_handle_t *handle, uart_intr_callback_t handler);
/**
 * Register tx ready interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: tx ready interrupt handler
 */
void uart_register_tx_handler(uart_handle_t *handle, uart_intr_callback_t handler);
/**
 * Register error interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: error interrupt handler
 */
void uart_register_error_handler(uart_handle_t *handle, uart_intr_callback_t handler);
/**
 * Read bytes from UART port
 *
 * :param handle: handle of uart port
 * :param buf: receive buffer address
 * :param bufsize: buffer sizie
 */
size_t uart_read(uart_handle_t *handle, void *buf, size_t bufsize);
/**
 * Write data to UART port
 *
 * :param handle: handle of uart port
 * :param data: data buffer address
 * :param datasize: data size to send
 */
size_t uart_write(uart_handle_t *handle, const void *data, size_t datasize);
/**
 * The default UART interrupt handler
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :note: Define :c:macro:`UART_MAX_DRIVER_HANDLE` to `0` not to occupy any
 *        memory if you don't use the default interrupt hander but your own.
 */
void uart_default_isr(uart_port_t uartp);

#if defined(UNITTEST)
void uart_teardown(void);
#endif

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_HAL_H */
