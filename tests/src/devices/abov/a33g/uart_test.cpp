#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/uart.h"
#include "a33g.h"

extern "C" {
static struct uart_stub reg0, reg1, reg2, reg3;

struct uart_stub * const UART0 = &reg0;
struct uart_stub * const UART1 = &reg1;
struct uart_stub * const UART2 = &reg2;
struct uart_stub * const UART3 = &reg3;
}

TEST_GROUP(uart) {
	void setup(void) {
		mock().ignoreOtherCalls();

		set_initial_state();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}

	void set_initial_state(void) {
		UART0->LSR = UART1->LSR = UART2->LSR = UART3->LSR = 0x60;
		UART0->IIR = UART1->IIR = UART2->IIR = UART3->IIR = 1;
		uart_ll_reset(PERIPH_UART0);
		uart_ll_reset(PERIPH_UART1);
		uart_ll_reset(PERIPH_UART2);
		uart_ll_reset(PERIPH_UART3);
	}
};

TEST(uart, get_event_ShouldReturnLRS) {
	uint32_t expected = 0xA55A;
	UART0->LSR = expected >> 8;
	UART0->IIR = expected & 0xff;
	LONGS_EQUAL(expected | UART_EVENT_TX_READY, uart_ll_get_event(PERIPH_UART0));
}

TEST(uart, enable_irq_ShouldSetDrieInIER_WhenRxGiven) {
	uart_ll_enable_irq(PERIPH_UART0, UART_IRQ_RX);
	LONGS_EQUAL(1, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearDrieInIER_WhenRxGiven) {
	UART0->IER = 1;
	uart_ll_disable_irq(PERIPH_UART0, UART_IRQ_RX);
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, enable_irq_ShouldSetDrieInIER_WhenTxGiven) {
	uart_ll_enable_irq(PERIPH_UART0, UART_IRQ_TX_READY);
	LONGS_EQUAL(2, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearDrieInIER_WhenTxGiven) {
	UART0->IER = 2;
	uart_ll_disable_irq(PERIPH_UART0, UART_IRQ_TX_READY);
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, enable_irq_ShouldSetMultiIntr_WhenMixedGiven) {
	uart_ll_enable_irq(PERIPH_UART0, (uart_irq_t)(UART_IRQ_RX | UART_IRQ_TX_READY));
	LONGS_EQUAL(3, UART0->IER);
}

TEST(uart, disable_irq_ShouldClearMultiIntr_WhenMixedGiven) {
	UART0->IER = 3;
	uart_ll_disable_irq(PERIPH_UART0, (uart_irq_t)(UART_IRQ_RX | UART_IRQ_TX_READY));
	LONGS_EQUAL(0, UART0->IER);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When38400With8MhzPclkGiven) {
	uart_ll_set_baudrate(PERIPH_UART0, 38400, 8000000);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x06, UART0->DLL);
	LONGS_EQUAL(0x82, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When38400With75MhzPclkGiven) {
	uart_ll_set_baudrate(PERIPH_UART0, 38400, 75000000);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x3d, UART0->DLL);
	LONGS_EQUAL(0x09, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When115200With75MhzPclkGiven) {
	uart_ll_set_baudrate(PERIPH_UART0, 115200, 75000000);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0x14, UART0->DLL);
	LONGS_EQUAL(0x58, UART0->BFR);
}

TEST(uart, set_baudrate_ShouldSetBaudrate_When9600With75MhzPclkGiven) {
	uart_ll_set_baudrate(PERIPH_UART0, 9600, 75000000);
	LONGS_EQUAL(0x00, UART0->DLM);
	LONGS_EQUAL(0xf4, UART0->DLL);
	LONGS_EQUAL(0x24, UART0->BFR);
}

TEST(uart, get_rxd_ShouldReturnReceivedByte) {
	UART0->RBR = 0xA5;
	LONGS_EQUAL(0xA5, uart_ll_get_rxd(PERIPH_UART0));
}

TEST(uart, set_txd_ShouldWriteByteToTHR) {
	uart_ll_set_txd(PERIPH_UART0, 0xA5);
	LONGS_EQUAL(0xA5, UART0->THR);
}

TEST(uart, set_parity_ShoudSetParity_WhenParityOddGiven) {
	uart_ll_set_parity(PERIPH_UART0, UART_PARITY_ODD);
	LONGS_EQUAL(0x8, UART0->LCR);
}

TEST(uart, set_parity_ShoudSetParity_WhenParityEvenGiven) {
	uart_ll_set_parity(PERIPH_UART0, UART_PARITY_EVEN);
	LONGS_EQUAL(0x18, UART0->LCR);
}

TEST(uart, set_parity_ShoudDisableParity_WhenParityNoneGiven) {
	UART0->LCR = 0x18;
	uart_ll_set_parity(PERIPH_UART0, UART_PARITY_NONE);
	LONGS_EQUAL(0x10, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When1StopGiven) {
	UART0->LCR = 0x4;
	uart_ll_set_stopbits(PERIPH_UART0, UART_STOPBIT_1);
	LONGS_EQUAL(0, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When1_5StopGiven) {
	uart_ll_set_stopbits(PERIPH_UART0, UART_STOPBIT_1_5);
	LONGS_EQUAL(4, UART0->LCR);
}

TEST(uart, set_stopbits_ShouldSetStopbit_When2StopGiven) {
	uart_ll_set_stopbits(PERIPH_UART0, UART_STOPBIT_2);
	LONGS_EQUAL(4, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When8Given) {
	uart_ll_set_wordsize(PERIPH_UART0, UART_WORDSIZE_8);
	LONGS_EQUAL(3, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When7Given) {
	uart_ll_set_wordsize(PERIPH_UART0, UART_WORDSIZE_7);
	LONGS_EQUAL(2, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When6Given) {
	uart_ll_set_wordsize(PERIPH_UART0, UART_WORDSIZE_6);
	LONGS_EQUAL(1, UART0->LCR);
}

TEST(uart, set_wordsize_ShouldSetWordsize_When5Given) {
	UART0->LCR = 1;
	uart_ll_set_wordsize(PERIPH_UART0, UART_WORDSIZE_5);
	LONGS_EQUAL(0, UART0->LCR);
}

TEST(uart, has_rx_ShouldReturnFalse_WhenNoReceivedData) {
	LONGS_EQUAL(0, uart_ll_has_rx(PERIPH_UART0));
}
TEST(uart, has_rx_ShouldReturnTrue_WhenReceived) {
	UART0->LSR = 1;
	LONGS_EQUAL(1, uart_ll_has_rx(PERIPH_UART0));
}

TEST(uart, is_tx_ready_ShouldReturnTrue_WhenReady) {
	LONGS_EQUAL(1, uart_ll_is_tx_ready(PERIPH_UART0));
}
TEST(uart, is_tx_ready_ShouldReturnFalse_WhenBusy) {
	UART0->LSR = 0;
	LONGS_EQUAL(0, uart_ll_is_tx_ready(PERIPH_UART0));
}
