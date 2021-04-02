#ifndef ABOV_UART_HAL_H
#define ABOV_UART_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** UART port enumeration */
typedef enum {
	/***/
	UART_PORT_0,
	/***/
	UART_PORT_1,
	/***/
	UART_PORT_2,
	/***/
	UART_PORT_3,
} uart_port_t;

/** UART parity enumeration */
typedef enum {
	/***/
	UART_PARITY_NONE,
	/***/
	UART_PARITY_ODD,
	/***/
	UART_PARITY_EVEN,
} uart_parity_t;

/** UART stopbit enumeration */
typedef enum {
	/***/
	UART_STOPBIT_1,
	/***/
	UART_STOPBIT_1_5,
	/***/
	UART_STOPBIT_2,
} uart_stopbit_t;

/** UART wordsize enumeration */
typedef enum {
	/***/
	UART_WORDSIZE_8		= 8,
	/***/
	UART_WORDSIZE_7		= 7,
	/***/
	UART_WORDSIZE_6		= 6,
	/***/
	UART_WORDSIZE_5		= 5,
} uart_wordsize_t;

/**
 * Reset UART interface
 *
 * This function makes the given UART the reset default state.
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_reset(uart_port_t port);
/**
 * Enable UART
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_enable(uart_port_t port);
/**
 * Disable UART
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_disable(uart_port_t port);
/**
 * Read a byte from UART
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :return: * received byte on success
 *          * -1 when no received data
 * :note: This function is non-blocking.
 */
int uart_read_byte(uart_port_t port);
/**
 * Write a byte to UART
 *
 * This function will block until the byte gets written into the hold register.
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :param val: value to write
 */
void uart_write_byte(uart_port_t port, uint8_t val);
/**
 * Set UART baudrate
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :param baudrate: baudrate
 */
void uart_set_baudrate(uart_port_t port, uint32_t baudrate);
/**
 * Enable UART receiver interrupt
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_enable_rx_intr(uart_port_t port);
/**
 * Disable UART receiver interrupt
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_disable_rx_intr(uart_port_t port);
/**
 * Enable UART transmitter interrupt
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_enable_tx_intr(uart_port_t port);
/**
 * Disable UART transmitter interrupt
 *
 * :param port: a enum of :c:type:`uart_port_t`
 */
void uart_disable_tx_intr(uart_port_t port);
/**
 * Read UART status flag
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :return: status flag
 */
uint32_t uart_get_status(uart_port_t port);
/**
 * Set UART parity
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :param parity: a enum of :c:type:`uart_parity_t`
 */
void uart_set_parity(uart_port_t port, uart_parity_t parity);
/**
 * Set UART stopbits
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :param stopbit: a enum of :c:type:`uart_stopbit_t`
 */
void uart_set_stopbits(uart_port_t port, uart_stopbit_t stopbit);
/**
 * Set UART data bit length
 *
 * :param port: a enum of :c:type:`uart_port_t`
 * :param wordsize: a enum of :c:type:`uart_wordsize_t`
 */
void uart_set_wordsize(uart_port_t port, uart_wordsize_t wordsize);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_HAL_H */
