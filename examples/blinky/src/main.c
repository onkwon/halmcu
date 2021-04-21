#include "abov/hal/gpio.h"
#include "abov/delay.h"

#define LED_PORT		PERIPH_GPIOD
#define LED_PIN			1

int main(void)
{
#if 0
	gpio_open_output(LED_PORT, LED_PIN, GPIO_MODE_PUSHPULL);
#else
	struct gpio_cfg cfg = { GPIO_MODE_PUSHPULL, };
	gpio_open(LED_PORT, LED_PIN, &cfg);
#endif

	while (1) {
		gpio_write(LED_PORT, LED_PIN, 1);
		udelay(500000);
		gpio_write(LED_PORT, LED_PIN, 0);
		udelay(500000);
	}

	return 0;
}
