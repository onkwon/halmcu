#include "abov/system.h"
#include "abov/hal/gpio.h"
#include "abov/hal/adc.h"

static void myadc_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_ANALOG,
		.altfunc = true,
		.altfunc_number = 3,
	};
	gpio_open(PERIPH_GPIOA, 1, &cfg);
}

static void myadc_init(void)
{
	myadc_gpio_init();

	adc_enable(PERIPH_ADC);
	adc_set_mode(PERIPH_ADC, ADC_MODE_SINGLE_CONVERSION);
#if defined(DEFAULT_CONFIGURATION)
	adc_set_clock_frequency(PERIPH_ADC, 1000000,
			clk_get_peripheral_clock_source_frequency(PERIPH_ADC));
	adc_set_trigger(PERIPH_ADC, ADC_TRIGGER_MANUAL);
#endif
	adc_select_channel(PERIPH_ADC, ADC_CHANNEL_1);
}

int main(void)
{
	myadc_init();

	adc_start(PERIPH_ADC);

	while (!adc_is_completed(PERIPH_ADC)) {
		/* waiting */
	}

	uint32_t adc_result = adc_get_measurement(PERIPH_ADC);

	adc_clear_event(PERIPH_ADC, ADC_EVENT_COMPLETE);

	return 0;
}
