#include "abov/asm/arm/systick.h"
#include "abov/irq.h"
#include "abov/hal/gpio.h"

#define LED_PORT		PERIPH_GPIOD
#define LED_PIN			1

int main(void)
{
	gpio_open_output(LED_PORT, LED_PIN, GPIO_MODE_PUSHPULL);

	systick_set_frequency(1);
	systick_clear();
	systick_start();

	while (1) {
		/* hang */
	}

	return 0;
}

void ISR_systick(void)
{
	static int led = 0;
	led ^= 1;
	gpio_write(LED_PORT, LED_PIN, led);
}
