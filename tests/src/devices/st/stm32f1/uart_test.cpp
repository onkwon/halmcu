#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/uart.h"
#include "stm32f1.h"
#include "assert.h"

extern "C" {
static USART_Type reg1, reg2, reg3, reg4, reg5;
USART_Type * const USART1 = &reg1;
USART_Type * const USART2 = &reg2;
USART_Type * const USART3 = &reg3;
USART_Type * const UART4 = &reg4;
USART_Type * const UART5 = &reg5;
static RCC_Type rcc_reg;
RCC_Type * const RCC = &rcc_reg;
}

void assert_override(void) {
	mock().actualCall(__func__);
}

TEST_GROUP(UART) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(USART1, 0, sizeof(*USART1));
		memset(USART2, 0, sizeof(*USART2));
		memset(USART3, 0, sizeof(*USART3));
		memset(UART4, 0, sizeof(*UART4));
		memset(UART5, 0, sizeof(*UART5));
		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(UART, reset_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_reset(PERIPH_GPIOA);
}

TEST(UART, has_rx_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_has_rx(PERIPH_GPIOA);
}
TEST(UART, has_rx_ShouldReturnFalse_WhenNothingReceived) {
	LONGS_EQUAL(0, uart_has_rx(PERIPH_USART1));
}
TEST(UART, has_rx_ShouldReturnTrue_WhenDataReceived) {
	USART2->SR = 1U << 5;
	LONGS_EQUAL(1, uart_has_rx(PERIPH_USART2));
}

TEST(UART, is_tx_ready_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_is_tx_ready(PERIPH_GPIOA);
}
TEST(UART, is_tx_ready_ShouldReturnTrue_WhenReadyToTransmit) {
	USART3->SR = 1U << 7;
	LONGS_EQUAL(1, uart_is_tx_ready(PERIPH_USART3));
}
TEST(UART, is_tx_ready_ShouldReturnTrue_WhenBusy) {
	LONGS_EQUAL(0, uart_is_tx_ready(PERIPH_UART4));
}

TEST(UART, get_rxd_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_get_rxd(PERIPH_GPIOA);
}
TEST(UART, get_rxd_ShouldReturnReceivedData) {
	USART1->DR = 0xa5;
	LONGS_EQUAL(0xa5, uart_get_rxd(PERIPH_USART1));
	LONGS_EQUAL(0, uart_get_rxd(PERIPH_UART5));
}

TEST(UART, set_txd_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_set_txd(PERIPH_GPIOA, 0);
}
TEST(UART, set_txd_ShouldSetDR) {
	uart_set_txd(PERIPH_USART1, 0x5a);
	LONGS_EQUAL(0x5a, USART1->DR);
	uart_set_txd(PERIPH_USART1, 0xa5);
	LONGS_EQUAL(0xa5, USART1->DR);
}

TEST(UART, set_baudrate_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	uart_set_baudrate(PERIPH_GPIOA, 115200, 1000000);
}
TEST(UART, set_baudrate_ShouldSetBRR_WhenPlckIs72MHz) {
	uart_set_baudrate(PERIPH_USART1, 2400, 72000000);
	LONGS_EQUAL(0x7530, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 9600, 72000000);
	LONGS_EQUAL(0x1d4c, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 115200, 72000000);
	LONGS_EQUAL(0x271, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 921600, 72000000);
	LONGS_EQUAL(0x4e, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 2250000, 72000000);
	LONGS_EQUAL(0x20, USART1->BRR);
}
TEST(UART, set_baudrate_ShouldSetBRR_WhenPlckIs36MHz) {
	uart_set_baudrate(PERIPH_USART1, 2400, 36000000);
	LONGS_EQUAL(0x3a98, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 9600, 36000000);
	LONGS_EQUAL(0xea6, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 115200, 36000000);
	LONGS_EQUAL(0x138, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 921600, 36000000);
	LONGS_EQUAL(0x27, USART1->BRR);
	uart_set_baudrate(PERIPH_USART1, 2250000, 36000000);
	LONGS_EQUAL(0x10, USART1->BRR);
}

TEST(UART, enable_irq_ShouldSetCR1) {
	uart_enable_irq(PERIPH_USART1, UART_IRQ_RX);
	LONGS_EQUAL(1 << 5, USART1->CR1);
	uart_enable_irq(PERIPH_USART2, (uart_irq_t)(UART_IRQ_RX | UART_IRQ_TX_READY));
	LONGS_EQUAL((1 << 5) | (1 << 7), USART2->CR1);
}

TEST(UART, disable_irq_ShouldClearCR1) {
	uart_enable_irq(PERIPH_USART1, UART_IRQ_RX);
	uart_enable_irq(PERIPH_USART2, (uart_irq_t)(UART_IRQ_RX | UART_IRQ_TX_READY));
	uart_disable_irq(PERIPH_USART1, UART_IRQ_RX);
	uart_disable_irq(PERIPH_USART2, (uart_irq_t)(UART_IRQ_RX | UART_IRQ_TX_READY));
	LONGS_EQUAL(0, USART1->CR1);
	LONGS_EQUAL(0, USART2->CR1);
}

TEST(UART, get_event_ShouldReturnSR) {
	USART1->SR = 0x1;
	LONGS_EQUAL(UART_EVENT_ERROR, uart_get_event(PERIPH_USART1));
	USART1->SR = 0xf;
	LONGS_EQUAL(UART_EVENT_ERROR, uart_get_event(PERIPH_USART1));
	USART1->SR = 0x20;
	LONGS_EQUAL(UART_EVENT_RX, uart_get_event(PERIPH_USART1));
	USART1->SR = 0x80;
	LONGS_EQUAL(UART_EVENT_TX_READY, uart_get_event(PERIPH_USART1));
}

TEST(UART, set_parity_ShouldSetCR1) {
	uart_set_parity(PERIPH_USART1, UART_PARITY_EVEN);
	LONGS_EQUAL(0x400, USART1->CR1);
	uart_set_parity(PERIPH_USART1, UART_PARITY_ODD);
	LONGS_EQUAL(0x600, USART1->CR1);
	uart_set_parity(PERIPH_USART1, UART_PARITY_NONE);
	LONGS_EQUAL(0x200, USART1->CR1);
}

TEST(UART, set_stopbits_ShouldSetCR2) {
	uart_set_stopbits(PERIPH_USART1, UART_STOPBIT_2);
	LONGS_EQUAL(0x2000, USART1->CR2);
	uart_set_stopbits(PERIPH_USART1, UART_STOPBIT_1_5);
	LONGS_EQUAL(0x3000, USART1->CR2);
	uart_set_stopbits(PERIPH_USART1, UART_STOPBIT_1);
	LONGS_EQUAL(0, USART1->CR2);
}

TEST(UART, set_wordsize_ShouldSetCR1) {
	uart_set_wordsize(PERIPH_USART1, UART_WORDSIZE_9);
	LONGS_EQUAL(0x1000, USART1->CR1);
	uart_set_wordsize(PERIPH_USART1, UART_WORDSIZE_8);
	LONGS_EQUAL(0, USART1->CR1);
}

TEST(UART, start_ShouldSetCR1) {
	uart_start(PERIPH_USART1);
	LONGS_EQUAL(0x200c, USART1->CR1);
}

TEST(UART, stop_ShouldClearCR1) {
	uart_start(PERIPH_USART1);
	uart_stop(PERIPH_USART1);
	LONGS_EQUAL(0, USART1->CR1);
}
