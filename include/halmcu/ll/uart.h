#ifndef HALMCU_UART_LL_H
#define HALMCU_UART_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/uart.h"

/**
 * @brief Reset UART interface
 *
 * This function makes the given UART the reset default state.
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 */
void uart_ll_reset(periph_t port);
/***/
bool uart_ll_has_rx(periph_t port);
/***/
bool uart_ll_is_tx_ready(periph_t port);
/***/
int uart_ll_get_rxd(periph_t port);
/***/
void uart_ll_set_txd(periph_t port, uint32_t value);
/**
 * @brief Enable UART interrupts
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] irqs a mix enum of @ref uart_event_t
 */
void uart_ll_enable_irq(periph_t port, uart_irq_t irqs);
/**
 * @brief Disable UART interrupts
 *
 * @param[in] port a peripheral enumerated in @ref periph_t
 * @param[in] irqs a mix enum of @ref uart_event_t
 */
void uart_ll_disable_irq(periph_t port, uart_irq_t irqs);
void uart_ll_start(periph_t port);
void uart_ll_stop(periph_t port);

void uart_ll_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk);
uart_event_t uart_ll_get_event(periph_t port);
void uart_ll_clear_event(periph_t port, uart_event_t events);
void uart_ll_set_parity(periph_t port, uart_parity_t parity);
void uart_ll_set_stopbits(periph_t port, uart_stopbit_t stopbit);
void uart_ll_set_wordsize(periph_t port, uart_wordsize_t wordsize);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_UART_LL_H */
