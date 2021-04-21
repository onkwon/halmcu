#include "abov/hal/gpio.h"
#include "abov/delay.h"

#define LED_PORT		PERI_GPIOD
#define LED_PIN			1

int main(void)
{
	gpio_open(LED_PORT, LED_PIN, GPIO_MODE_PUSHPULL);

	while (1) {
		gpio_write(LED_PORT, LED_PIN, 1);
		udelay(500000);
		gpio_write(LED_PORT, LED_PIN, 0);
		udelay(500000);
	}

	return 0;
}
