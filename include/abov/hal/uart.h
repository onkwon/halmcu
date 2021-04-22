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
typedef void (*uart_irq_callback_t)(uint32_t flags);

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
	char _size[sizeof(struct uart_cfg) + 24];
#else
	char _size[sizeof(struct uart_cfg) + 12];
#endif
	long _align;
} uart_handle_t;

/**
 * Initialize UART port with given configuration
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param cfg: configuration
 * :param handle: handle of uart port
 * :return: `true` on success
 */
bool uart_init(periph_t uart, const struct uart_cfg *cfg, uart_handle_t *handle);
/**
 * Deinitialize UART port
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 */
void uart_deinit(periph_t uart);
/**
 * Read bytes from UART port
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param buf: receive buffer address
 * :param bufsize: buffer sizie
 */
size_t uart_read(periph_t uart, void *buf, size_t bufsize);
/**
 * Write data to UART port
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param data: data buffer address
 * :param datasize: data size to send
 */
size_t uart_write(periph_t uart, const void *data, size_t datasize);
/**
 * Read a byte from UART
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :return: received byte
 */
int uart_read_byte(periph_t port);
/**
 * Read a byte from UART
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :return: * received byte on success
 *          * -1 when no received data
 * :note: This function is non-blocking.
 */
int uart_read_byte_nonblock(periph_t port);
/**
 * Write a byte to UART
 *
 * This function will block until the byte gets written into the hold register.
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param val: value to write
 */
void uart_write_byte(periph_t port, uint8_t val);
/**
 * Register rx interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: rx interrupt handler
 */
void uart_register_rx_handler(uart_handle_t *handle, uart_irq_callback_t handler);
/**
 * Register tx ready interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: tx ready interrupt handler
 */
void uart_register_tx_handler(uart_handle_t *handle, uart_irq_callback_t handler);
/**
 * Register error interrupt handler
 *
 * :param handle: handle of uart port
 * :param handler: error interrupt handler
 */
void uart_register_error_handler(uart_handle_t *handle, uart_irq_callback_t handler);
/**
 * The default UART interrupt handler
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param handle: handle of uart port
 */
void uart_default_isr(periph_t uart, const uart_handle_t *handle);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_HAL_H */
