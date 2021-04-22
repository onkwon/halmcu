#include "abov/ll/uart.h"

#include <assert.h>
#include <stddef.h>
#include <stdbool.h>

#include "abov/bitop.h"
#include "abov/compiler.h"

#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define PARITY_POS			4U
#define PARITY_EN_POS			3U
#define STOPBIT_POS			2U
#define WORDSIZE_POS			0U

#define RDR				1U
#define THRE				(1U << 5)
#define TEMT				(1U << 6)
#define DRIE				1U
#define THREIE				(1U << 1)

static UART_Type *get_uart_from_port(periph_t port)
{
	switch (port) {
	case PERIPH_UART0:
		return UART0;
	case PERIPH_UART1:
		return UART1;
	case PERIPH_UART2:
		return UART2;
	case PERIPH_UART3:
		return UART3;
	default:
		assert(0);
		return UART0;
	}
}

void uart_reset(periph_t port)
{
	UART_Type *uart = get_uart_from_port(port);

	uart->IER = 0;
	unused(uart->IIR);
	unused(uart->LSR);
	uart->FCR = 0;
	uart->SCR = 0;
	uart->DTR = 0;
	uart->LCR = 0x80;
	uart->DLL = 0;
	uart->DLM = 0;
	uart->BFR = 0;
	uart->LCR = 0;
}

bool uart_has_rx(periph_t port)
{
	return !!(get_uart_from_port(port)->LSR & RDR);
}

bool uart_is_tx_ready(periph_t port)
{
	return (get_uart_from_port(port)->LSR & (THRE | TEMT)) == (THRE | TEMT);
}

int uart_get_rxd(periph_t port)
{
	return (int)get_uart_from_port(port)->RBR;
}

void uart_set_txd(periph_t port, uint32_t value)
{
	get_uart_from_port(port)->THR = value;
}

void uart_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk)
{
	UART_Type *uart = get_uart_from_port(port);

	uint32_t n = pclk / 2;
	uint32_t d = 16 * baudrate;
	uint32_t y = n / d;
	uint32_t r = n - y * d;

	uint8_t bfr = (uint8_t)(r * 256 / d);
	uint8_t dlm = (uint8_t)(y >> 8);
	uint8_t dll = (uint8_t)y;

	uint32_t lcr = uart->LCR;
	uart->LCR = 0x80;
	uart->DLM = (uint32_t)dlm;
	uart->DLL = (uint32_t)dll;
	uart->BFR = (uint32_t)bfr;
	uart->LCR = lcr;
}

void uart_enable_irq(periph_t port, uart_irq_t events)
{
	UART_Type *uart = get_uart_from_port(port);

	if (events & UART_IRQ_RX) {
		uart->IER |= DRIE;
	}
	if (events & UART_IRQ_TX_READY) {
		uart->IER |= THREIE;
	}
}

void uart_disable_irq(periph_t port, uart_irq_t events)
{
	UART_Type *uart = get_uart_from_port(port);

	if (events & UART_IRQ_RX) {
		uart->IER &= ~DRIE;
	}
	if (events & UART_IRQ_TX_READY) {
		uart->IER &= ~THREIE;
	}
}

uart_event_t uart_get_event(periph_t port)
{
	const UART_Type *uart = get_uart_from_port(port);

	uint32_t iir = uart->IIR;
	uint32_t lsr = uart->LSR;
	uint32_t flags = 0;

	switch (iir & 7) {
	case 2: /* transmit ready */
		flags |= UART_EVENT_TX_READY;
		break;
	case 3: /* error */
		flags |= UART_EVENT_ERROR;
		break;
	case 4: /* rx */
		flags |= UART_EVENT_RX;
		break;
	default:
		break;
	}

	return (uart_event_t)(flags | (lsr << 8) | iir);
}

void uart_clear_event(periph_t port, uart_event_t events)
{
	unused(port);
	unused(events);
}

void uart_set_parity(periph_t port, uart_parity_t parity)
{
	UART_Type *uart = get_uart_from_port(port);

	bitop_clear(&uart->LCR, PARITY_EN_POS);

	if (!!parity) {
		bitop_clean_set_with_mask(&uart->LCR,
				PARITY_POS, 1U, (uint32_t)parity - 1);
		bitop_set(&uart->LCR, PARITY_EN_POS);
	}
}

void uart_set_stopbits(periph_t port, uart_stopbit_t stopbit)
{
	UART_Type *uart = get_uart_from_port(port);

	uint32_t val = (stopbit == UART_STOPBIT_1)? 0 : 1;
	bitop_clean_set_with_mask(&uart->LCR,
			STOPBIT_POS, 1U, val);
}

void uart_set_wordsize(periph_t port, uart_wordsize_t wordsize)
{
	assert(wordsize < UART_WORDSIZE_9);

	UART_Type *uart = get_uart_from_port(port);

	bitop_clean_set_with_mask(&uart->LCR,
			WORDSIZE_POS, 3U, (uint32_t)wordsize-5);
}

void uart_start(periph_t port)
{
	unused(port);
}

void uart_stop(periph_t port)
{
	unused(port);
}
