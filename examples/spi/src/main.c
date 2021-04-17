#include "abov/system.h"
#include "abov/hal/gpio.h"
#include "abov/irq.h"

#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/ll/spi.h"

static void myspi_gpio_init(void)
{
	gpio_open(GPIOB + 10, GPIO_MODE_PUSHPULL); // SS
	gpio_set_altfunc(GPIOB + 10, 1);
	gpio_open(GPIOB + 11, GPIO_MODE_PUSHPULL); // SCK
	gpio_set_altfunc(GPIOB + 11, 1);
	gpio_open(GPIOB + 12, GPIO_MODE_PUSHPULL); // MOSI
	gpio_set_altfunc(GPIOB + 12, 1);
	gpio_open(GPIOB + 13, GPIO_MODE_INPUT); // MISO
	gpio_set_altfunc(GPIOB + 13, 1);
}

static void myspi_init(void)
{
	myspi_gpio_init();

	pwr_enable_peripheral(PERI_SPI0);
	clk_enable_peripheral(PERI_SPI0);

	spi_reset(PERI_SPI0);
	spi_set_mode(PERI_SPI0, SPI_MODE_MASTER);
	spi_set_frequency(PERI_SPI0, 1000000, clk_get_pclk_frequency());
	spi_set_clock_phase(PERI_SPI0, 0);
	spi_set_clock_polarity(PERI_SPI0, 0);
	spi_set_bitorder(PERI_SPI0, false);
	spi_set_data_width(PERI_SPI0, 8);

	spi_disable_chip_select(PERI_SPI0);
	spi_set_loopback(PERI_SPI0, true);
}

int main(void)
{
	myspi_init();
	spi_start(PERI_SPI0);

	while (!(spi_get_event(PERI_SPI0) & SPI_EVENT_TX_COMPLETE)) { /* waiting */ }
	spi_set_txd(PERI_SPI0, 0xA5);

	while (!(spi_get_event(PERI_SPI0) & SPI_EVENT_RX)) { /* waiting */ }

	uint32_t received = spi_get_rxd(PERI_SPI0);
	(void)received;

	return 0;
}
