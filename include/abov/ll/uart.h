#ifndef ABOV_UART_LL_H
#define ABOV_UART_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/periph/periph.h"

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

/** UART event enumeration */
typedef enum {
	/***/
	UART_EVENT_BIT			= 28,
	/***/
	UART_EVENT_RX			= (1U << (UART_EVENT_BIT+0)),
	/***/
	UART_EVENT_TX_READY		= (1U << (UART_EVENT_BIT+1)),
	/***/
	UART_EVENT_ERROR		= (1U << (UART_EVENT_BIT+2)),
	/***/
	UART_EVENT_MASK			=
		(UART_EVENT_RX | UART_EVENT_TX_READY | UART_EVENT_ERROR),
} uart_event_t;

/**
 * Reset UART interface
 *
 * This function makes the given UART the reset default state.
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 */
void uart_reset(periph_t port);
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
 * Set UART baudrate
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param baudrate: baudrate
 * :param pclk: pclk
 */
void uart_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk);
/**
 * Enable UART interrupts
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param events: a mix enum of :c:type:`uart_event_t`
 */
void uart_enable_irq(periph_t port, uart_event_t events);
/**
 * Disable UART interrupts
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param events: a mix enum of :c:type:`uart_event_t`
 */
void uart_disable_irq(periph_t port, uart_event_t events);
/**
 * Read UART event flag
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :return: :c:type:`uart_event_t`
 */
uart_event_t uart_get_event(periph_t port);
/**
 * Clear UART event flag
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param events: a mix enum of :c:type:`uart_event_t`
 */
void uart_clear_event(periph_t port, uart_event_t events);
/**
 * Set UART parity
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param parity: a enum of :c:type:`uart_parity_t`
 */
void uart_set_parity(periph_t port, uart_parity_t parity);
/**
 * Set UART stopbits
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param stopbit: a enum of :c:type:`uart_stopbit_t`
 */
void uart_set_stopbits(periph_t port, uart_stopbit_t stopbit);
/**
 * Set UART data bit length
 *
 * :param port: a peripheral enumerated in :c:type:`periph_t`
 * :param wordsize: a enum of :c:type:`uart_wordsize_t`
 */
void uart_set_wordsize(periph_t port, uart_wordsize_t wordsize);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_LL_H */
