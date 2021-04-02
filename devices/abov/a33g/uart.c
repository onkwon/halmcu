#include "abov/hal/uart.h"

#include <assert.h>
#include <stddef.h>

#include "abov/bitop.h"
#include "abov/compiler.h"

#include "abov/hal/pwr.h"
#include "abov/hal/clk.h"
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

static UART_Type *get_uart_from_port(uart_port_t port)
{
	switch (port) {
	case UARTp0:
		return UART0;
	case UARTp1:
		return UART1;
	case UARTp2:
		return UART2;
	case UARTp3:
		return UART3;
	default:
		return NULL;
	}
}

static bool is_tx_busy(const UART_Type *uart)
{
	return (uart->LSR & (THRE | TEMT)) != (THRE | TEMT);
}

uint32_t uart_get_status(uart_port_t port)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);
	return (uart->LSR << 8) | uart->IIR;
}

void uart_enable_rx_intr(uart_port_t port)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);
	uart->IER |= DRIE;
}

void uart_disable_rx_intr(uart_port_t port)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);
	uart->IER &= ~DRIE;
}

void uart_set_baudrate(uart_port_t port, uint32_t baudrate)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	uint32_t n = clk_get_pclk_frequency() / 2;
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

int uart_read_byte(uart_port_t port)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	if (uart->LSR & RDR) {
		return (int)uart->RBR;
	}

	return -1;
}

void uart_write_byte(uart_port_t port, uint8_t val)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	while (is_tx_busy(uart)) {
		/* wait for transmit buffer gets available */
	}

	uart->THR = (uint32_t)val;
}

void uart_reset(uart_port_t port)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	while (is_tx_busy(uart)) {
		/* wait until finising ongoing tx */
	}

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

void uart_set_parity(uart_port_t port, uart_parity_t parity)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	bitop_clear(&uart->LCR, PARITY_EN_POS);

	if (!!parity) {
		bitop_clean_set_with_mask(&uart->LCR,
				PARITY_POS, 1U << PARITY_POS, (uint32_t)parity - 1);
		bitop_set(&uart->LCR, PARITY_EN_POS);
	}
}

void uart_set_stopbits(uart_port_t port, uart_stopbit_t stopbit)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	uint32_t val = (stopbit == UART_STOPBIT_1)? 0 : 1;
	bitop_clean_set_with_mask(&uart->LCR,
			STOPBIT_POS, 1U << STOPBIT_POS, val);
}

void uart_set_wordsize(uart_port_t port, uart_wordsize_t wordsize)
{
	UART_Type *uart = get_uart_from_port(port);
	assert(uart != NULL);

	bitop_clean_set_with_mask(&uart->LCR,
			WORDSIZE_POS, 3U << WORDSIZE_POS, (uint32_t)wordsize-5);
}

void uart_enable(uart_port_t port)
{
	uint32_t peri = (uint32_t)port + PERIPHERAL_UART0;
	assert(peri >= PERIPHERAL_UART0 && peri <= PERIPHERAL_UART3);

	pwr_enable_peripheral(peri);
	clk_enable_peripheral(peri);
}

void uart_disable(uart_port_t port)
{
	uint32_t peri = (uint32_t)port + PERIPHERAL_UART0;
	assert(peri >= PERIPHERAL_UART0 && peri <= PERIPHERAL_UART3);

	clk_disable_peripheral(peri);
	pwr_disable_peripheral(peri);
}
