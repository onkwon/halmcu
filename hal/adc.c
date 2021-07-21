#include "halmcu/hal/adc.h"
#include "halmcu/periph/pwr.h"
#include "halmcu/periph/clk.h"

void adc_enable(periph_t adc)
{
	pwr_enable_peripheral(adc);
	clk_enable_peripheral(adc);

	adc_enable_clock(adc);
}

void adc_disable(periph_t adc)
{
	adc_disable_clock(adc);

	clk_disable_peripheral(adc);
	pwr_disable_peripheral(adc);
}
