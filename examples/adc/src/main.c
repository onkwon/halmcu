#include "abov/system.h"
#include "abov/hal/gpio.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/ll/adc.h"

static void myadc_gpio_init(void)
{
	gpio_open(GPIOA + 1, GPIO_MODE_ANALOG);
	gpio_set_altfunc(GPIOA + 1, 3);
}

static void myadc_init(void)
{
	myadc_gpio_init();

	pwr_enable_peripheral(PERI_ADC);
	clk_enable_peripheral(PERI_ADC);

	adc_enable(PERI_ADC);
	adc_set_mode(PERI_ADC, ADC_MODE_NORMAL);
	adc_set_clock_frequency(PERI_ADC, 1000000, clk_get_pclk_frequency());
	adc_set_trigger(PERI_ADC, ADC_TRIGGER_MANUAL);
	adc_select_channel(PERI_ADC, ADC_CHANNEL_1);
}

int main(void)
{
	myadc_init();

	adc_start(PERI_ADC);

	while (!adc_is_completed(PERI_ADC)) {
		/* waiting */
	}

	uint32_t adc_result = adc_read(PERI_ADC);

	adc_clear_event(PERI_ADC, ADC_EVENT_COMPLETE);

	return 0;
}
