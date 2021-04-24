#ifndef ABOV_UART_H
#define ABOV_UART_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/periph/periph.h"

/** UART parity enumeration */
typedef enum {
	UART_PARITY_NONE,
	UART_PARITY_ODD,
	UART_PARITY_EVEN,
} uart_parity_t;

/** UART stopbit enumeration */
typedef enum {
	UART_STOPBIT_1,
	UART_STOPBIT_1_5,
	UART_STOPBIT_2,
} uart_stopbit_t;

/** UART wordsize enumeration */
typedef enum {
	UART_WORDSIZE_8				= 8,
	UART_WORDSIZE_9				= 9,
	UART_WORDSIZE_7				= 7,
	UART_WORDSIZE_6				= 6,
	UART_WORDSIZE_5				= 5,
} uart_wordsize_t;

/** UART irq enumeration */
typedef enum {
	UART_IRQ_NONE				= 0,
	UART_IRQ_RX				= (1U << 0),
	UART_IRQ_TX_READY			= (1U << 1),
	UART_IRQ_MASK				= 0x3,
} uart_irq_t;

/** UART event enumeration */
typedef enum {
	UART_EVENT_BIT				= 28,
	UART_EVENT_RX				= (1U << (UART_EVENT_BIT+0)),
	UART_EVENT_TX_READY			= (1U << (UART_EVENT_BIT+1)),
	UART_EVENT_ERROR			= (1U << (UART_EVENT_BIT+2)),
	UART_EVENT_MASK				=
		(UART_EVENT_RX | UART_EVENT_TX_READY | UART_EVENT_ERROR),
} uart_event_t;

/**
 * @brief Reset UART interface
 *
 * This function makes the given UART the reset default state.
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 */
void uart_reset(periph_t port);
/***/
bool uart_has_rx(periph_t port);
/***/
bool uart_is_tx_ready(periph_t port);
/***/
int uart_get_rxd(periph_t port);
/***/
void uart_set_txd(periph_t port, uint32_t value);
/**
 * @brief Set UART baudrate
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] baudrate baudrate
 * @param[in] pclk pclk
 */
void uart_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk);
/**
 * @brief Enable UART interrupts
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] events a mix enum of @ref uart_event_t
 */
void uart_enable_irq(periph_t port, uart_irq_t irqs);
/**
 * @brief Disable UART interrupts
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] events a mix enum of @ref uart_event_t
 */
void uart_disable_irq(periph_t port, uart_irq_t irqs);
/**
 * @brief Read UART event flag
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @return @ref uart_event_t
 */
uart_event_t uart_get_event(periph_t port);
/**
 * @brief Clear UART event flag
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] events a mix enum of @ref uart_event_t
 */
void uart_clear_event(periph_t port, uart_event_t events);
/**
 * @brief Set UART parity
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] parity a enum of @ref uart_parity_t
 */
void uart_set_parity(periph_t port, uart_parity_t parity);
/**
 * @brief Set UART stopbits
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] stopbit a enum of @ref uart_stopbit_t
 */
void uart_set_stopbits(periph_t port, uart_stopbit_t stopbit);
/**
 * @brief Set UART data bit length
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] wordsize a enum of @ref uart_wordsize_t
 */
void uart_set_wordsize(periph_t port, uart_wordsize_t wordsize);
/***/
void uart_start(periph_t port);
/***/
void uart_stop(periph_t port);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_UART_H */
