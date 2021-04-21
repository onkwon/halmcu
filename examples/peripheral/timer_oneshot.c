#include "abov/system.h"
#include "abov/hal/uart.h"
#include "abov/hal/gpio.h"
#include "abov/hal/clk.h"

static uart_handle_t uart0_handle;

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

#include "abov/hal/timer.h"
#include "abov/irq.h"
static void mytimer_init(void)
{
	pwr_enable_peripheral(PERIPH_TIMER0);
	clk_enable_peripheral(PERIPH_TIMER0);

	timer_reset(PERIPH_TIMER0);
	timer_set_mode(PERIPH_TIMER0, TIMER_MODE_ONESHOT);
	timer_set_clock_divider(PERIPH_TIMER0, 3); // 16MHz/64 = 250KHz
	timer_set_prescaler(PERIPH_TIMER0, 250 - 1); // 1-msec tick
	timer_set_cc(PERIPH_TIMER0, 0, 3000 - 1); // 1-sec match
	timer_set_cc(PERIPH_TIMER0, 1, 5000 - 1); // 2-sec overflow
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_CC_0);
	timer_enable_irq(PERIPH_TIMER0, TIMER_EVENT_CC_1);

	irq_set_priority(IRQ_TIMER0, 3);
	irq_enable(IRQ_TIMER0);

	timer_start(PERIPH_TIMER0);
}

int main(void)
{
	system_clock_init();

	uart_gpio_init();
	uart_init(PERIPH_UART0, &(struct uart_cfg) {
			.wordsize = UART_WORDSIZE_8,
			.stopbit = UART_STOPBIT_1,
			.parity = UART_PARITY_NONE,
			.baudrate = 115200, },
			&uart0_handle);

	uart_write(PERIPH_UART0, "Hello, World!\r\n", 15);

	mytimer_init();

	while (1) {
		uint8_t ch;
		size_t received = uart_read(PERIPH_UART0, &ch, sizeof(ch));
		if (received > 0) {
			uart_write(PERIPH_UART0, &ch, sizeof(ch));
		}
	}

	return 0;
}

void ISR_TIMER0(void)
{
	uart_write(PERIPH_UART0, "INT!\r\n", 6);
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_CC_0);
	timer_clear_event(PERIPH_TIMER0, TIMER_EVENT_CC_1);
}
