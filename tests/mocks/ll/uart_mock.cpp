#include "CppUTestExt/MockSupport.h"

#include "abov/ll/uart.h"

void uart_reset(periph_t port)
{
	mock().actualCall(__func__).withParameter("port", port);
}

void uart_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("baudrate", baudrate)
		.withParameter("pclk", pclk);
}

void uart_set_wordsize(periph_t port, uart_wordsize_t wordsize)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("wordsize", wordsize);
}

void uart_set_stopbits(periph_t port, uart_stopbit_t stopbit)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("stopbit", stopbit);
}

void uart_set_parity(periph_t port, uart_parity_t parity)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("parity", parity);
}

void uart_enable_irq(periph_t port, uart_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("events", events);
}

void uart_clear_event(periph_t port, uart_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("events", events);
}

int uart_read_byte_nonblock(periph_t port)
{
	return mock().actualCall(__func__)
		.withParameter("port", port)
		.returnIntValueOrDefault(-1);
}

void uart_write_byte(periph_t port, uint8_t val)
{
	mock().actualCall(__func__)
		.withParameter("port", port);

	(void)val;
}

uart_event_t uart_get_event(periph_t port) {
	return (uart_event_t)mock().actualCall(__func__)
		.withParameter("port", port)
		.returnUnsignedIntValueOrDefault(0);
}
