#include "abov/system.h"
#include "abov/hal/uart.h"
#include "abov/hal/gpio.h"
#include "abov/irq.h"
#include "abov/delay.h"

#include "abov/ll/clk.h"

#define UART0_RX_PIN			(GPIOC + 8)
#define UART0_TX_PIN			(GPIOC + 9)

static uart_handle_t uart0_handle;

#include "printf.h"
void _putchar(char character)
{
	uart_write(PERI_UART0, &character, 1);
}

#include "libmcu/shell.h"
static size_t shell_read(void *buf, size_t bufsize)
{
	size_t result = uart_read(PERI_UART0, buf, bufsize);
	return result;
}

static size_t shell_write(const void *data, size_t datasize)
{
	size_t result = uart_write(PERI_UART0, data, datasize);
	return result;
}

#include "abov/hal/i2c.h"
#include "abov/ll/pwr.h"
#define I2C_SCL				(GPIOB + 14)
#define I2C_SDA				(GPIOB + 15)
static void myi2c_gpio_init(void)
{
	gpio_open(I2C_SCL, GPIO_MODE_OPENDRAIN);
	gpio_open(I2C_SDA, GPIO_MODE_OPENDRAIN);
	gpio_set_altfunc(I2C_SCL, 1);
	gpio_set_altfunc(I2C_SDA, 1);
}
static void myi2c_init(void)
{
	myi2c_gpio_init();
}

uint32_t bccr,ccr, mr, wdt, pcsr;
static void system_clock_init(void)
{
	bccr = *(volatile uint32_t *)0x4000003c;
	ccr = *(volatile uint32_t *)0x40000030;
	mr = *(volatile uint32_t *)0x40000004;
	wdt = *(volatile uint32_t *)0x40000408;
	pcsr = *(volatile uint32_t *)0x40000040;

#if 1
	clk_enable_source(CLK_HSI);
#if 1
	clk_set_pll_frequency(CLK_PLL, CLK_HSI, 16000000);
#else
	gpio_open(GPIOC + 14, GPIO_MODE_ANALOG); // XTALO
	gpio_set_altfunc(GPIOC + 14, 1);
	gpio_open(GPIOC + 15, GPIO_MODE_ANALOG); // XTALI
	gpio_set_altfunc(GPIOC + 15, 1);
	clk_enable_source(CLK_HSE);
	clk_set_pll_frequency(CLK_PLL, CLK_HSE, 16000000);
#endif
	clk_enable_pll();
	clk_set_source(CLK_PLL);
	while (!clk_is_pll_locked()) ;
#endif
}

static void system_init(void)
{
	system_clock_init();
}

static void uart_gpio_init(void)
{
	gpio_open(UART0_RX_PIN, GPIO_MODE_INPUT_PULLUP);
	gpio_open(UART0_TX_PIN, GPIO_MODE_PUSHPULL);
	gpio_set_altfunc(UART0_RX_PIN, 1);
	gpio_set_altfunc(UART0_TX_PIN, 1);
}

int _sbrk_r;
int main(void)
{
	system_init();

	uart_gpio_init();
	uart_init(PERI_UART0, &(struct uart_cfg) {
			.wordsize = UART_WORDSIZE_8,
			.stopbit = UART_STOPBIT_1,
			.parity = UART_PARITY_NONE,
			.baudrate = 115200, },
			&uart0_handle);

	uart_write(PERI_UART0, "Hello, World!\r\n", 15);
	printf("hclk %u, pclk %u\r\n", clk_get_hclk_frequency(), clk_get_pclk_frequency());

	myi2c_init();

	pwr_enable_peripheral(PERI_I2C0);
	clk_enable_peripheral(PERI_I2C0);
	i2c_reset(PERI_I2C0);
	i2c_set_frequency(PERI_I2C0, 100000, clk_get_pclk_frequency());

	//i2c_enable_irq(PERI_I2C0);
	//irq_enable(PERI_TO_IRQ(PERI_I2C0));

	i2c_start(PERI_I2C0, 0x69, 0);
	i2c_write_byte(PERI_I2C0, 0xf);
	i2c_start(PERI_I2C0, 0x69, 1);
	int rxd = i2c_read_byte(PERI_I2C0, false);
	i2c_stop(PERI_I2C0);

#if 0
	i2c_start(PERI_I2C0, 0x69, 0);
	i2c_write_byte(PERI_I2C0, 0x20);
	i2c_write_byte(PERI_I2C0, 0xf);
	i2c_stop(PERI_I2C0);
#endif

	i2c_start(PERI_I2C0, 0x69, 0);
	i2c_write_byte(PERI_I2C0, 0x20);
	i2c_start(PERI_I2C0, 0x69, 1);
	//int rxdata = i2c_read_byte(PERI_I2C0, false);
	int rxdata = i2c_read_byte(PERI_I2C0, true);
	rxdata = i2c_read_byte(PERI_I2C0, true);
	rxdata = i2c_read_byte(PERI_I2C0, false);
	i2c_stop(PERI_I2C0);

	printf("Recieved : %x\r\n", rxd);
	printf("Recieved : %x\r\n", rxdata);

	const shell_io_t io = {
		.read = shell_read,
		.write = shell_write,
	};
	shell_run(&io);

	while (1) {
		uint8_t ch;
		size_t received = uart_read(PERI_UART0, &ch, sizeof(ch));
		if (received > 0) {
			uart_write(PERI_UART0, &ch, sizeof(ch));
		}
	}

	return 0;
}

void ISR_I2C0(void)
{
	printf("INT\r\n");
#if 0
	switch (i2c_run_fsm()) {
	case I2C_FSM_ERROR:
		i2c_stop(PERI_I2C0);
		break;
	case I2C_FSM_START_SEND:
		i2c_set_txd(PERI_I2C0, 0xf);
		break;
	case I2C_FSM_START_RECV:
		break;
	case I2C_FSM_SENDING:
		i2c_set_txd(PERI_I2C0, 0xd3);
		i2c_disable_ack(PERI_I2C0);
		i2c_set_start(PERI_I2C0);
		break;
	case I2C_FSM_RECEIVING:
		printf("@gcall>tend|receiving: %x\r\n", i2c_read_byte(PERI_I2C0));
		i2c_stop(PERI_I2C0);
		break;
	default:
		break;
	}
#endif
#if 0
	uint32_t event = i2c_get_event(PERI_I2C0);
	if (event & 0x80) { //GCALL
		if (!(event & 1)) {
			printf("@gcall>noack\r\n");
			i2c_stop(PERI_I2C0);
		} else if (event & 2) { //송신모드
			printf("@gcall>tmode\r\n");
i2c_set_txd(PERI_I2C0, 0xf);
		} else { // 수신모드
		}
	} else if (event & 0x40) { //TEND
		if (event & 0x2) { //송신모드
			// if no ack, stop
			printf("@gcall>tend|tmode\r\n");
i2c_set_txd(PERI_I2C0, 0xd3);
//i2c_enable_ack(PERI_I2C0);
i2c_set_start(PERI_I2C0);
		} else { //수신모드
			// 마지막 바이트면 nak
			printf("@gcall>tend|receiving: %x\r\n", i2c_read_byte(PERI_I2C0));
i2c_stop(PERI_I2C0);
		}
	}
#endif

	volatile uint32_t *sr = (volatile uint32_t *)0x40000a08;
	*sr = 0xff;
}
