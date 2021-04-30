#include "abov/hal/adc.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

void adc_enable(periph_t adc)
{
	pwr_enable_peripheral(adc);
	clk_enable_peripheral(adc);

	adc_activate(adc);
}

void adc_disable(periph_t adc)
{
	adc_deactivate(adc);

	clk_disable_peripheral(adc);
	pwr_disable_peripheral(adc);
}
