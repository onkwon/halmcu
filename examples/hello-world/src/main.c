#include "abov/system.h"
#include "abov/hal/uart.h"
#include "abov/hal/gpio.h"
#include "abov/hal/clk.h"
#include "abov/hal/wdt.h"

#define UART0_RX_PIN			(GPIOC + 8)
#define UART0_TX_PIN			(GPIOC + 9)

static uart_handle_t uart0_handle;

static void system_clock_init(void)
{
	clk_enable_source(CLK_INTERNAL_OSC_16MHZ);
	clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 16000000);
	clk_enable_pll();
	clk_set_source(CLK_PLL);
	while (!clk_is_pll_locked()) ;
}

static void uart_gpio_init(void)
{
	gpio_open(UART0_RX_PIN, GPIO_MODE_INPUT_PULLUP);
	gpio_open(UART0_TX_PIN, GPIO_MODE_PUSHPULL);
	gpio_set_altfunc(UART0_RX_PIN, 1);
	gpio_set_altfunc(UART0_TX_PIN, 1);
}

int main(void)
{
	system_clock_init();

	uart_gpio_init();
	uart_init(&uart0_handle, UART_PORT_0, &(struct uart_cfg) {
			.wordsize = UART_WORDSIZE_8,
			.stopbit = UART_STOPBIT_1,
			.parity = UART_PARITY_NONE,
			.baudrate = 115200, });

	uart_write(&uart0_handle, "Hello, World!\r\n", 15);

	while (1) {
		uint8_t ch;
		size_t received = uart_read(&uart0_handle, &ch, sizeof(ch));
		if (received > 0) {
			uart_write(&uart0_handle, &ch, sizeof(ch));
		}
	}

	return 0;
}
