#include "CppUTestExt/MockSupport.h"

#include "halmcu/ll/uart.h"

void uart_ll_reset(periph_t port)
{
	mock().actualCall(__func__).withParameter("port", port);
}

bool uart_ll_has_rx(periph_t port)
{
	return mock().actualCall(__func__).withParameter("port", port)
		.returnBoolValueOrDefault(false);
}

bool uart_ll_is_tx_ready(periph_t port)
{
	return mock().actualCall(__func__).withParameter("port", port)
		.returnBoolValueOrDefault(false);
}

int uart_ll_get_rxd(periph_t port)
{
	return mock().actualCall(__func__).withParameter("port", port)
		.returnIntValueOrDefault(-1);
}

void uart_ll_set_txd(periph_t port, uint32_t value)
{
	mock().actualCall(__func__).withParameter("port", port);
	(void)value;
}

void uart_ll_set_baudrate(periph_t port, uint32_t baudrate, uint32_t pclk)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("baudrate", baudrate)
		.withParameter("pclk", pclk);
}

void uart_ll_enable_irq(periph_t port, uart_irq_t irqs)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("irqs", irqs);
}

uart_event_t uart_ll_get_event(periph_t port) {
	return (uart_event_t)mock().actualCall(__func__)
		.withParameter("port", port)
		.returnUnsignedIntValueOrDefault(0);
}

void uart_ll_clear_event(periph_t port, uart_event_t events)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("events", events);
}

void uart_ll_set_parity(periph_t port, uart_parity_t parity)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("parity", parity);
}

void uart_ll_set_stopbits(periph_t port, uart_stopbit_t stopbit)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("stopbit", stopbit);
}

void uart_ll_set_wordsize(periph_t port, uart_wordsize_t wordsize)
{
	mock().actualCall(__func__)
		.withParameter("port", port)
		.withParameter("wordsize", wordsize);
}

void uart_ll_start(periph_t port)
{
	mock().actualCall(__func__).withParameter("port", port);
}

void uart_ll_stop(periph_t port)
{
	mock().actualCall(__func__).withParameter("port", port);
}
