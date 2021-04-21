#include "abov/hal/gpio.h"
#include "abov/hal/timer.h"
#include "abov/irq.h"

/*****************************************************/
#include "abov/hal/clk.h"
#include "abov/hal/uart.h"
#include "printf.h"

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

	uart_init(PERIPH_UART0, &(struct uart_cfg) {
			.wordsize = UART_WORDSIZE_8,
			.stopbit = UART_STOPBIT_1,
			.parity = UART_PARITY_NONE,
			.baudrate = 115200, },
			&uart0_handle);
	uart_write(PERIPH_UART0, "Hello, World!\r\n", 15);
}

void _putchar(char character)
{
	uart_write(PERIPH_UART0, &character, 1);
}
/*****************************************************/

static void set_target_timer_clock_source(void)
{
	timer_set_clock_divider(PERIPH_TIMER0, 3); // 16MHz/64 = 250KHz
}

static void pwm_timer_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_PUSHPULL,
		.altfunc = true,
		.altfunc_number = 1,
	};
	gpio_open(PERIPH_GPIOB, 0, &cfg);
}

// SG90 servo pulse width: 500 ~ 2400us
static void pwm_timer_init(void)
{
	pwm_timer_gpio_init();

	timer_init(PERIPH_TIMER0, &(struct timer_cfg) {
			.mode = TIMER_MODE_PWM,
			.frequency = 100000,
			.set_clock_source = set_target_timer_clock_source,
			});
	timer_set_polarity(PERIPH_TIMER0, 1); // set high when timer gets zero
	timer_set_cc(PERIPH_TIMER0, 1, 2000 - 1); // GRB: period, 50hz
	timer_set_cc(PERIPH_TIMER0, 0, 100 - 1); // GRA: duty, 1.5ms

	timer_start(PERIPH_TIMER0);
}

#include "abov/delay.h"
int main(void)
{
	system_clock_init();
	uart_gpio_init();

	pwm_timer_init();

	while (1) {
		/* hang */
		for (uint32_t i = 60; i < 280; i++) {
			timer_set_cc(PERIPH_TIMER0, 0, i); // GRA: duty, 1.5ms
			udelay(100000);
		}
		udelay(5000000);
	}

	return 0;
}
