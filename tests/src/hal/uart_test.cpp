#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/uart.h"
#include "abov/hal/irq.h"

#include <string.h>

static void intr_handler(uint32_t flags) {
	mock().actualCall(__func__).withParameter("flags", flags);
}

void uart_reset(uart_port_t port) {
	mock().actualCall(__func__).withParameter("port", port);
}
void uart_set_baudrate(uart_port_t port, uint32_t baudrate) {
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("baudrate", baudrate);
}
void uart_set_wordsize(uart_port_t port, uart_wordsize_t wordsize) {
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("wordsize", wordsize);
}
void uart_set_stopbits(uart_port_t port, uart_stopbit_t stopbit) {
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("stopbit", stopbit);
}
void uart_set_parity(uart_port_t port, uart_parity_t parity) {
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("parity", parity);
}
void uart_enable_rx_intr(uart_port_t port) {
	mock().actualCall(__func__).withParameter("port", port);
}
void uart_enable_tx_intr(uart_port_t port) {
	mock().actualCall(__func__).withParameter("port", port);
}
void irq_enable(irq_t irq) {
	mock().actualCall(__func__).withParameter("irq", irq);
}
void irq_disable(irq_t irq) {
	mock().actualCall(__func__).withParameter("irq", irq);
}
uint32_t uart_get_status(uart_port_t port) {
	return mock().actualCall(__func__).withParameter("port", port)
		.returnUnsignedIntValueOrDefault(0);
}
int uart_read_byte_nonblock(uart_port_t port) {
	return mock().actualCall(__func__).withParameter("port", port)
		.returnIntValueOrDefault(-1);
}
void uart_write_byte(uart_port_t port, uint8_t val) {
	mock().actualCall(__func__).withParameter("port", port);
	(void)val;
}
void uart_enable(uart_port_t port) {
	mock().actualCall(__func__).withParameter("port", port);
}
void uart_disable(uart_port_t port) {
	mock().actualCall(__func__).withParameter("port", port);
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
		uart_teardown();

		mock().checkExpectations();
		mock().clear();
	}
};

TEST(uart_driver, init_ShouldReturnFalse_WhenNullObjGiven) {
	LONGS_EQUAL(0, uart_init(NULL, UART_PORT_0, NULL));
	LONGS_EQUAL(0, uart_init(&default_handle, UART_PORT_0, NULL));
	LONGS_EQUAL(0, uart_init(NULL, UART_PORT_0, &default_cfg));
}
TEST(uart_driver, init_ShouldReturnTrue_WhenAllGivenParamVaild) {
	mock().expectOneCall("uart_enable").withParameter("port", UART_PORT_0);
	mock().expectOneCall("uart_reset").withParameter("port", UART_PORT_0);
	mock().expectOneCall("uart_set_baudrate")
		.withParameter("port", UART_PORT_0)
		.withParameter("baudrate", 115200);
	mock().expectOneCall("uart_set_wordsize")
		.withParameter("port", UART_PORT_0)
		.withParameter("wordsize", 8);
	mock().expectOneCall("uart_set_stopbits")
		.withParameter("port", UART_PORT_0)
		.withParameter("stopbit", UART_STOPBIT_1);
	mock().expectOneCall("uart_set_parity")
		.withParameter("port", UART_PORT_0)
		.withParameter("parity", UART_PARITY_NONE);
	mock().expectNoCall("irq_enable");

	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
}
TEST(uart_driver, init_ShouldReturnFalse_WhenFailedAllocateHandle) {
	// UART_MAX_DRIVER_HANDLE == 3
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_1, &default_cfg));
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_2, &default_cfg));
	LONGS_EQUAL(0, uart_init(&default_handle, UART_PORT_3, &default_cfg));
}
TEST(uart_driver, init_ShouldReturnFalse_WhenCalledWithSamePort) {
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
	LONGS_EQUAL(0, uart_init(&default_handle, UART_PORT_0, &default_cfg));
}
TEST(uart_driver, init_ShouldEnableRxInterrupt_WhenRxInterruptGiven) {
	default_cfg.rx_interrupt = true;
	mock().expectOneCall("uart_enable_rx_intr").withParameter("port", UART_PORT_0);
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
}
TEST(uart_driver, init_ShouldEnableTxInterrupt_WhenTxInterruptGiven) {
	default_cfg.tx_interrupt = true;
	mock().expectOneCall("uart_enable_tx_intr").withParameter("port", UART_PORT_0);
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
}

TEST(uart_driver, deinit) {
	// UART_MAX_DRIVER_HANDLE == 3
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_0, &default_cfg));
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_1, &default_cfg));
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_2, &default_cfg));
	LONGS_EQUAL(0, uart_init(&default_handle, UART_PORT_3, &default_cfg));
	mock().expectOneCall("irq_disable").withParameter("irq", 14 + IRQ_FIXED);
	mock().expectOneCall("uart_disable").withParameter("port", UART_PORT_2);
	uart_deinit(&default_handle);
	LONGS_EQUAL(1, uart_init(&default_handle, UART_PORT_3, &default_cfg));
}

TEST(uart_driver, rx_interrupt_ShouldCallRxHandler) {
	default_cfg.rx_interrupt = true;
	mock().expectOneCall("irq_enable").withParameter("irq", 3 + IRQ_FIXED);
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	uart_register_rx_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_get_status").withParameter("port", UART_PORT_0)
		.andReturnValue(UART_EVENT_RX);
	mock().expectOneCall("intr_handler").withParameter("flags", UART_EVENT_RX);
	uart_default_isr(UART_PORT_0);
}
TEST(uart_driver, tx_interrupt_ShouldCallTxHandler) {
	default_cfg.tx_interrupt = true;
	mock().expectOneCall("irq_enable").withParameter("irq", 4 + IRQ_FIXED);
	uart_init(&default_handle, UART_PORT_1, &default_cfg);
	uart_register_tx_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_get_status").withParameter("port", UART_PORT_1)
		.andReturnValue(UART_EVENT_TX_READY);
	mock().expectOneCall("intr_handler").withParameter("flags", UART_EVENT_TX_READY);
	uart_default_isr(UART_PORT_1);
}
TEST(uart_driver, error_interrupt_ShouldCallErrorHandler) {
	uart_init(&default_handle, UART_PORT_2, &default_cfg);
	uart_register_error_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_get_status").withParameter("port", UART_PORT_2)
		.andReturnValue(UART_EVENT_ERROR);
	mock().expectOneCall("intr_handler").withParameter("flags", UART_EVENT_ERROR);
	uart_default_isr(UART_PORT_2);
}
TEST(uart_driver, error_interrupt_ShouldCallNothing_WhenNohandlerRegistered) {
	uart_init(&default_handle, UART_PORT_3, &default_cfg);
	mock().expectOneCall("uart_get_status").withParameter("port", UART_PORT_3)
		.andReturnValue(UART_EVENT_ERROR);
	mock().expectNoCall("intr_handler");
	uart_default_isr(UART_PORT_3);
}
TEST(uart_driver, multiple_interrupt_ShouldCallEachHandlers) {
	default_cfg.rx_interrupt = true;
	default_cfg.tx_interrupt = true;
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	uart_register_rx_handler(&default_handle, intr_handler);
	uart_register_tx_handler(&default_handle, intr_handler);
	uart_register_error_handler(&default_handle, intr_handler);
	mock().expectOneCall("uart_get_status").withParameter("port", UART_PORT_0)
		.andReturnValue(UART_EVENT_RX | UART_EVENT_TX_READY | UART_EVENT_ERROR);
	mock().expectNCalls(3, "intr_handler")
		.withParameter("flags", UART_EVENT_RX | UART_EVENT_TX_READY | UART_EVENT_ERROR);
	uart_default_isr(UART_PORT_0);
}

TEST(uart_driver, read_ShouldReturnZero_WhenNoDataReceived) {
	uint8_t c;
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	mock().expectOneCall("uart_read_byte_nonblock").withParameter("port", UART_PORT_0);
	LONGS_EQUAL(0, uart_read(&default_handle, &c, 1));
}
TEST(uart_driver, read_ShouldReturnReceivedData) {
	uint8_t buf[3];
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	mock().expectNCalls(3, "uart_read_byte_nonblock")
		.withParameter("port", UART_PORT_0)
		.andReturnValue(1);
	LONGS_EQUAL(3, uart_read(&default_handle, buf, sizeof(buf)));
	LONGS_EQUAL(1, buf[0]);
	LONGS_EQUAL(1, buf[1]);
	LONGS_EQUAL(1, buf[2]);
}
TEST(uart_driver, read_ShouldStopReading_WhenBufsizeShort) {
	uint8_t buf[1];
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	mock().expectNCalls(1, "uart_read_byte_nonblock")
		.withParameter("port", UART_PORT_0)
		.andReturnValue(1);
	LONGS_EQUAL(1, uart_read(&default_handle, buf, sizeof(buf)));
	LONGS_EQUAL(1, buf[0]);
}

TEST(uart_driver, wrte_ShouldWrite) {
	const char *data = "Hello, World!";
	size_t datalen = strlen(data);
	uart_init(&default_handle, UART_PORT_0, &default_cfg);
	mock().expectNCalls((unsigned int)datalen, "uart_write_byte")
		.withParameter("port", UART_PORT_0);
	LONGS_EQUAL(datalen, uart_write(&default_handle, data, datalen));
}
