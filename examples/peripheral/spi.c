#include "abov/system.h"
#include "abov/hal/gpio.h"
#include "abov/irq.h"

#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/ll/spi.h"

static void myspi_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_PUSHPULL,
		.altfunc = true,
		.altfunc_number = 1,
	};
	gpio_open(PERIPH_GPIOB, 10, &cfg); // SS
	gpio_open(PERIPH_GPIOB, 11, &cfg); // SCK
	gpio_open(PERIPH_GPIOB, 12, &cfg); // MOSI
	gpio_open(PERIPH_GPIOB, 13, &cfg); // MISO
}

static void myspi_init(void)
{
	myspi_gpio_init();

	pwr_enable_peripheral(PERIPH_SPI0);
	clk_enable_peripheral(PERIPH_SPI0);

	spi_reset(PERIPH_SPI0);
	spi_set_mode(PERIPH_SPI0, SPI_MODE_MASTER);
	spi_set_frequency(PERIPH_SPI0, 1000000, clk_get_pclk_frequency());
	spi_set_clock_phase(PERIPH_SPI0, 0);
	spi_set_clock_polarity(PERIPH_SPI0, 0);
	spi_set_bitorder(PERIPH_SPI0, false);
	spi_set_data_width(PERIPH_SPI0, 8);

	spi_disable_chip_select(PERIPH_SPI0);
	spi_set_loopback(PERIPH_SPI0, true);
}

int main(void)
{
	myspi_init();
	spi_start(PERIPH_SPI0);

	while (!(spi_get_event(PERIPH_SPI0) & SPI_EVENT_TX_COMPLETE)) { /* waiting */ }
	spi_set_txd(PERIPH_SPI0, 0xA5);

	while (!(spi_get_event(PERIPH_SPI0) & SPI_EVENT_RX)) { /* waiting */ }

	uint32_t received = spi_get_rxd(PERIPH_SPI0);
	(void)received;

	return 0;
}
