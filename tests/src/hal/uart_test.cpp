#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/uart.h"
#include "abov/irq.h"

#include <string.h>

static void intr_handler(uint32_t flags) {
	mock().actualCall(__func__).withParameter("flags", flags);
}

TEST_GROUP(uart_driver) {
	uart_handle_t default_handle;
	struct uart_cfg default_cfg = {
		.wordsize = UART_WORDSIZE_8,
		.stopbit = UART_STOPBIT_1,
		.parity = UART_PARITY_NONE,
		.baudrate = 115200,
		.rx_interrupt = false,
		.tx_interrupt = false,
	};

	void setup(void) {
		mock().ignoreOtherCalls();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(uart_driver, init_ShouldReturnFalse_WhenNullObjGiven) {
	LONGS_EQUAL(0, uart_init(PERIPH_UART0, NULL, NULL));
}
TEST(uart_driver, init_ShouldReturnTrue_WhenAllGivenParamVaild) {
	mock().expectOneCall("pwr_ll_enable_peripheral")
		.withParameter("peri", PERIPH_UART0);
	mock().expectOneCall("clk_ll_enable_peripheral")
		.withParameter("peri", PERIPH_UART0);
	mock().expectOneCall("uart_ll_reset").withParameter("port", PERIPH_UART0);
	mock().expectOneCall("clk_ll_get_peripheral_clock_source_frequency")
		.withParameter("peri", PERIPH_UART0)
		.andReturnValue(16000000);
	mock().expectOneCall("uart_ll_set_baudrate")
		.withParameter("port", PERIPH_UART0)
		.withParameter("baudrate", 115200)
		.withParameter("pclk", 16000000);
	mock().expectOneCall("uart_ll_set_wordsize")
		.withParameter("port", PERIPH_UART0)
		.withParameter("wordsize", 8);
	mock().expectOneCall("uart_ll_set_stopbits")
		.withParameter("port", PERIPH_UART0)
		.withParameter("stopbit", UART_STOPBIT_1);
	mock().expectOneCall("uart_ll_set_parity")
		.withParameter("port", PERIPH_UART0)
		.withParameter("parity", UART_PARITY_NONE);
	mock().expectNoCall("irq_enable");

	LONGS_EQUAL(1, uart_init(PERIPH_UART0, &default_cfg, &default_handle));
}
TEST(uart_driver, init_ShouldEnableRxInterrupt_WhenRxInterruptGiven) {
	default_cfg.rx_interrupt = true;
	mock().expectOneCall("uart_ll_enable_irq")
		.withParameter("port", PERIPH_UART0)
		.withParameter("irqs", UART_IRQ_RX);
	LONGS_EQUAL(1, uart_init(PERIPH_UART0, &default_cfg, &default_handle));
}
TEST(uart_driver, init_ShouldEnableTxInterrupt_WhenTxInterruptGiven) {
	default_cfg.tx_interrupt = true;
	mock().expectOneCall("uart_ll_enable_irq")
		.withParameter("port", PERIPH_UART0)
		.withParameter("irqs", UART_IRQ_TX_READY);
	LONGS_EQUAL(1, uart_init(PERIPH_UART0, &default_cfg, &default_handle));
}

TEST(uart_driver, deinit) {
	LONGS_EQUAL(1, uart_init(PERIPH_UART0, &default_cfg, &default_handle));
	mock().expectOneCall("irq_disable").withParameter("irq", 3 + IRQ_FIXED);
	mock().expectOneCall("clk_ll_disable_peripheral")
		.withParameter("peri", PERIPH_UART0);
	mock().expectOneCall("pwr_ll_disable_peripheral")
		.withParameter("peri", PERIPH_UART0);
	uart_deinit(PERIPH_UART0);
}

TEST(uart_driver, rx_interrupt_ShouldCallRxHandler) {
	default_cfg.rx_interrupt = true;
	mock().expectOneCall("irq_enable").withParameter("irq", 3 + IRQ_FIXED);
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	uart_register_rx_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_ll_get_event")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(UART_EVENT_RX);
	mock().expectOneCall("intr_handler")
		.withParameter("flags", UART_EVENT_RX);
	uart_default_isr(PERIPH_UART0, &default_handle);
}
TEST(uart_driver, tx_interrupt_ShouldCallTxHandler) {
	default_cfg.tx_interrupt = true;
	mock().expectOneCall("irq_enable").withParameter("irq", 4 + IRQ_FIXED);
	uart_init(PERIPH_UART1, &default_cfg, &default_handle);
	uart_register_tx_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_ll_get_event")
		.withParameter("port", PERIPH_UART1)
		.andReturnValue(UART_EVENT_TX_READY);
	mock().expectOneCall("intr_handler")
		.withParameter("flags", UART_EVENT_TX_READY);
	uart_default_isr(PERIPH_UART1, &default_handle);
}
TEST(uart_driver, error_interrupt_ShouldCallErrorHandler) {
	uart_init(PERIPH_UART2, &default_cfg, &default_handle);
	uart_register_error_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_ll_get_event")
		.withParameter("port", PERIPH_UART2)
		.andReturnValue(UART_EVENT_ERROR);
	mock().expectOneCall("intr_handler")
		.withParameter("flags", UART_EVENT_ERROR);
	uart_default_isr(PERIPH_UART2, &default_handle);
}
TEST(uart_driver, error_interrupt_ShouldCallNothing_WhenNohandlerRegistered) {
	uart_init(PERIPH_UART3, &default_cfg, &default_handle);
	mock().expectOneCall("uart_ll_get_event")
		.withParameter("port", PERIPH_UART3)
		.andReturnValue(UART_EVENT_ERROR);
	mock().expectNoCall("intr_handler");
	uart_default_isr(PERIPH_UART3, &default_handle);
}
TEST(uart_driver, multiple_interrupt_ShouldCallEachHandlers) {
	default_cfg.rx_interrupt = true;
	default_cfg.tx_interrupt = true;
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	uart_register_rx_handler(&default_handle, intr_handler);
	uart_register_tx_handler(&default_handle, intr_handler);
	uart_register_error_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_ll_get_event")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(UART_EVENT_RX | UART_EVENT_TX_READY |
				UART_EVENT_ERROR);
	mock().expectNCalls(3, "intr_handler")
		.withParameter("flags", UART_EVENT_RX | UART_EVENT_TX_READY | UART_EVENT_ERROR);
	uart_default_isr(PERIPH_UART0, &default_handle);
}

TEST(uart_driver, read_ShouldReturnZero_WhenNoDataReceived) {
	uint8_t c;
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	mock().expectOneCall("uart_ll_has_rx")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(false);
	LONGS_EQUAL(0, uart_read(PERIPH_UART0, &c, 1));
}
TEST(uart_driver, read_ShouldReturnReceivedData) {
	uint8_t buf[3];
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	mock().expectNCalls(3, "uart_ll_has_rx")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(true);
	mock().expectNCalls(3, "uart_ll_get_rxd")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(1);
	LONGS_EQUAL(3, uart_read(PERIPH_UART0, buf, sizeof(buf)));
	LONGS_EQUAL(1, buf[0]);
	LONGS_EQUAL(1, buf[1]);
	LONGS_EQUAL(1, buf[2]);
}
TEST(uart_driver, read_ShouldStopReading_WhenBufsizeShort) {
	uint8_t buf[1];
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	mock().expectNCalls(1, "uart_ll_has_rx")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(true);
	mock().expectNCalls(1, "uart_ll_get_rxd")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(1);
	LONGS_EQUAL(1, uart_read(PERIPH_UART0, buf, sizeof(buf)));
	LONGS_EQUAL(1, buf[0]);
}

TEST(uart_driver, wrte_ShouldWrite) {
	const char *data = "Hello, World!";
	size_t datalen = strlen(data);
	uart_init(PERIPH_UART0, &default_cfg, &default_handle);
	mock().expectNCalls((unsigned int)datalen, "uart_ll_is_tx_ready")
		.withParameter("port", PERIPH_UART0)
		.andReturnValue(true);
	mock().expectNCalls((unsigned int)datalen, "uart_ll_set_txd")
		.withParameter("port", PERIPH_UART0);
	LONGS_EQUAL(datalen, uart_write(PERIPH_UART0, data, datalen));
}
