#include "abov/system.h"
#include "abov/hal/uart.h"
#include "abov/hal/gpio.h"
#include "abov/hal/clk.h"
#include "abov/hal/irq.h"

#define UART0_RX_PIN			(GPIOC + 8)
#define UART0_TX_PIN			(GPIOC + 9)
//#define POLLING

static uart_handle_t uart0_handle;

static void uart_rx_hander(uint32_t flags)
{
	(void)flags;
	uint8_t c;
	uart_read(&uart0_handle, &c, 1);
	uart_write(&uart0_handle, "Received!\r\n", 11);
}

static void system_clock_init(void)
{
	clk_enable_source(CLK_HSI);
	clk_set_pll_frequency(CLK_PLL, CLK_HSI, 16000000);
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
			.baudrate = 115200,
#if !defined(POLLING)
			.rx_interrupt = true,
#endif
			});
	uart_register_rx_handler(&uart0_handle, uart_rx_hander);

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

void ISR_UART0(void)
{
	uart_default_isr(UART_PORT_0, &uart0_handle);
}
