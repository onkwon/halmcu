#include "halmcu/ll/uart.h"

#include <stddef.h>

#include "halmcu/bitop.h"
#include "halmcu/compiler.h"
#include "halmcu/assert.h"
#include "stm32f1.h"
#include "exti.h"

static USART_Type *get_instance(periph_t port)
{
	switch (port) {
	case PERIPH_USART1:
		return USART1;
	case PERIPH_USART2:
		return USART2;
	case PERIPH_USART3:
		return USART3;
	case PERIPH_UART4:
		return UART4;
	case PERIPH_UART5:
		return UART5;
	default:
		assert(0);
		return USART1;
	}
}

static uint32_t brr2reg(uint32_t baudrate, uint32_t pclk)
{
	/* 25 * 4 = 100; to not lose the result below the decimal point */
	uint32_t fraction = (pclk * 25) / (baudrate * 4);
	uint32_t mantissa = fraction / 100; /* to get the actual integer part */
	fraction = fraction - (mantissa * 100); /* to get the fraction part */
	fraction = ((fraction << 4/* sampling */) + 50/* round up */) / 100;

	return (mantissa << 4) | (fraction & 0xf);
}

void uart_ll_reset(periph_t port)
{
	volatile uint32_t *reg = &RCC->APB1RSTR;
	uint32_t pos = 17;

	switch (port) {
	case PERIPH_USART1:
		reg = &RCC->APB2RSTR;
		pos = 14;
		break;
	case PERIPH_USART2:
		pos = 17;
		break;
	case PERIPH_USART3:
		pos = 18;
		break;
	case PERIPH_UART4:
		pos = 19;
		break;
	case PERIPH_UART5:
		pos = 20;
		break;
	default:
		assert(0);
		break;
	}

	bitop_set(reg, pos);
	bitop_clear(reg, pos);
}

bool uart_ll_has_rx(periph_t port)
{
	return !!(get_instance(port)->SR & (1U << 5)/*RXNE*/);
}

bool uart_ll_is_tx_ready(periph_t port)
{
	return !!(get_instance(port)->SR & (1U << 7)/*TXE*/);
}

int uart_ll_get_rxd(periph_t port)
{
	return (int)get_instance(port)->DR;
}

void uart_ll_set_txd(periph_t port, uint32_t value)
{
	get_instance(port)->DR = value;
}

void uart_ll_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk)
{
	assert(baudrate != 0);
	assert(baudrate <= pclk);
	get_instance(port)->BRR = brr2reg(baudrate, pclk);
}

void uart_ll_enable_irq(periph_t port, uart_irq_t irqs)
{
	if (irqs & UART_IRQ_RX) {
		bitop_set(&get_instance(port)->CR1, 5/*RXNEIE*/);
	}
	if (irqs & UART_IRQ_TX_READY) {
		bitop_set(&get_instance(port)->CR1, 7/*TXEIE*/);
	}
}

void uart_ll_disable_irq(periph_t port, uart_irq_t irqs)
{
	if (irqs & UART_IRQ_RX) {
		bitop_clear(&get_instance(port)->CR1, 5/*RXNEIE*/);
	}
	if (irqs & UART_IRQ_TX_READY) {
		bitop_clear(&get_instance(port)->CR1, 7/*TXEIE*/);
	}
}

uart_event_t uart_ll_get_event(periph_t port)
{
	uint32_t flags = get_instance(port)->SR;
	uint32_t events = 0;

	if (flags & 0xf) { /* overrun, noise, framing, and parity error */
		events |= UART_EVENT_ERROR;
	}
	if (flags & (1U << 5)) { /* RXNE */
		events |= UART_EVENT_RX;
	}
	if (flags & (1U << 7)) { /* TXE */
		events |= UART_EVENT_TX_READY;
	}

	return (uart_event_t)events;
}

void uart_ll_clear_event(periph_t port, uart_event_t events)
{
	unused(port);
	unused(events);
}

void uart_ll_set_parity(periph_t port, uart_parity_t parity)
{
	if (parity == UART_PARITY_NONE) {
		bitop_clear(&get_instance(port)->CR1, 10/*PCE*/);
		return;
	}

	uint32_t val = 2;
	if (parity == UART_PARITY_ODD) {
		val = 3;
	}
	bitop_clean_set_with_mask(&get_instance(port)->CR1, 9, 3, val);
}

void uart_ll_set_stopbits(periph_t port, uart_stopbit_t stopbit)
{
	uint32_t val = 0;

	if (stopbit == UART_STOPBIT_1_5) {
		val = 3;
	} else if (stopbit == UART_STOPBIT_2) {
		val = 2;
	}

	bitop_clean_set_with_mask(&get_instance(port)->CR2, 12, 3, val);
}

void uart_ll_set_wordsize(periph_t port, uart_wordsize_t wordsize)
{
	assert(wordsize >= UART_WORDSIZE_8 && wordsize <= UART_WORDSIZE_9);
	bitop_clean_set_with_mask(&get_instance(port)->CR1,
			12, 1, wordsize & 1);
}

void uart_ll_start(periph_t port)
{
	bitop_set(&get_instance(port)->CR1, 2/*RE*/);
	bitop_set(&get_instance(port)->CR1, 3/*TE*/);
	bitop_set(&get_instance(port)->CR1, 13/*UE*/);
}

void uart_ll_stop(periph_t port)
{
	bitop_clear(&get_instance(port)->CR1, 2/*RE*/);
	bitop_clear(&get_instance(port)->CR1, 3/*TE*/);
	bitop_clear(&get_instance(port)->CR1, 13/*UE*/);
}
