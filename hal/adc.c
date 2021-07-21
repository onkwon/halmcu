#include "halmcu/hal/adc.h"
#include "halmcu/ll/pwr.h"
#include "halmcu/ll/clk.h"

void adc_enable(periph_t adc)
{
	pwr_ll_enable_peripheral(adc);
	clk_ll_enable_peripheral(adc);

	adc_enable_clock(adc);
}

void adc_disable(periph_t adc)
{
	adc_disable_clock(adc);

	clk_ll_disable_peripheral(adc);
	pwr_ll_disable_peripheral(adc);
}
