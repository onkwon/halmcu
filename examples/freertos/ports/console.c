#include "console.h"

#include "halmcu/hal/uart.h"
#include "halmcu/hal/gpio.h"
#include "halmcu/irq.h"

#include "libmcu/ringbuf.h"
#include "libmcu/assert.h"

#include "printf.h"

void _putchar(char character)
{
	uart_write(PERIPH_USART1, &character, 1);
}

static uart_handle_t uart0_handle;
static ringbuf_static_t recvbuf_handle;
static uint8_t recvbuf[64];

static void uart_rx_handler(uint32_t flags)
{
	(void)flags;
	uint8_t c;
	uart_read(PERIPH_USART1, &c, 1);
	ringbuf_write(&recvbuf_handle, &c, 1);
}

static void uart_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_PUSHPULL,
		.altfunc = true,
	};
	gpio_open(PERIPH_GPIOA, 9, &cfg); // TX

	cfg.mode = GPIO_MODE_INPUT_PULLUP;
	gpio_open(PERIPH_GPIOA, 10, &cfg); // RX
}

size_t console_read(void *buf, size_t bufsize)
{
	return ringbuf_read(&recvbuf_handle, 0, buf, bufsize);
}

size_t console_write(const void *data, size_t datasize)
{
	const char *p = (const char *)data;
	for (size_t i = 0; i < datasize; i++) {
		uart_write(PERIPH_USART1, &p[i], 1);
	}

	return datasize;
}

void console_init(void)
{
	bool ok = ringbuf_create_static(&recvbuf_handle, recvbuf, sizeof(recvbuf));
	assert(ok);

	uart_gpio_init();
	uart_init(PERIPH_USART1, &(struct uart_cfg) {
			.wordsize = UART_WORDSIZE_8,
			.stopbit = UART_STOPBIT_1,
			.parity = UART_PARITY_NONE,
			.baudrate = 115200,
#if !defined(POLLING)
			.rx_interrupt = true,
#endif
			},
			&uart0_handle);
	uart_register_rx_handler(&uart0_handle, uart_rx_handler);
}

void ISR_USART1(void)
{
	uart_default_isr(PERIPH_USART1, &uart0_handle);
}
