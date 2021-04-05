#include "abov/drivers/gpio.h"
#include "abov/delay.h"

#define LED_PIN			(GPIOD + 1)

int main(void)
{
	gpio_open(LED_PIN, GPIO_MODE_PUSHPULL);

	while (1) {
		gpio_write(LED_PIN, 1);
		udelay(500000);
		gpio_write(LED_PIN, 0);
		udelay(500000);
	}

	return 0;
}
