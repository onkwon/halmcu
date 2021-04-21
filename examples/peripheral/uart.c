#include "abov/system.h"
#include "abov/irq.h"
#include "abov/hal/uart.h"
#include "abov/hal/gpio.h"
#include "abov/hal/clk.h"

//#define POLLING

static uart_handle_t uart0_handle;

static void uart_rx_handler(uint32_t flags)
{
	(void)flags;
	uint8_t c;
	uart_read(PERIPH_UART0, &c, 1);
	uart_write(PERIPH_UART0, "Received!\r\n", 11);
}

static void system_clock_init(void)
{
	clk_enable_source(CLK_HSI);
	clk_set_pll_frequency(CLK_PLL, CLK_HSI, 16000000);
	clk_start_pll();
	clk_set_source(CLK_PLL);
	while (!clk_is_pll_locked()) ;
}

static void uart_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_PUSHPULL,
		.altfunc = true,
		.altfunc_number = 1,
	};
	gpio_open(PERIPH_GPIOC, 9, &cfg); // TX

	cfg.mode = GPIO_MODE_INPUT_PULLUP;
	gpio_open(PERIPH_GPIOC, 8, &cfg); // RX
}

int main(void)
{
	system_clock_init();

	uart_gpio_init();
	uart_init(PERIPH_UART0, &(struct uart_cfg) {
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

	uart_write(PERIPH_UART0, "Hello, World!\r\n", 15);

	while (1) {
		uint8_t ch;
		size_t received = uart_read(PERIPH_UART0, &ch, sizeof(ch));
		if (received > 0) {
			uart_write(PERIPH_UART0, &ch, sizeof(ch));
		}
	}

	return 0;
}

void ISR_UART0(void)
{
	uart_default_isr(PERIPH_UART0, &uart0_handle);
}
