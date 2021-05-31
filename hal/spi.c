#include "abov/hal/spi.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

void spi_enable(periph_t spi)
{
	pwr_ll_enable_peripheral(spi);
	clk_ll_enable_peripheral(spi);
}

void spi_disable(periph_t spi)
{
	clk_ll_disable_peripheral(spi);
	pwr_ll_disable_peripheral(spi);
}

void spi_start(periph_t spi)
{
	spi_ll_start(spi);
}

void spi_stop(periph_t spi)
{
	while (spi_ll_is_busy(spi)) {
		/* waiting for ongoing job finished */
	}

	spi_ll_stop(spi);
}
