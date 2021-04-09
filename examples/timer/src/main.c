#include "abov/hal/gpio.h"
#include "abov/hal/timer.h"
#include "abov/irq.h"

#define LED_PIN			(GPIOD + 1)

int main(void)
{
	gpio_open(LED_PIN, GPIO_MODE_PUSHPULL);

	pwr_enable_peripheral(PERIPHERAL_TIMER0);
	clk_enable_peripheral(PERIPHERAL_TIMER0);

	timer_reset(PERIPHERAL_TIMER0);
	timer_set_mode(PERIPHERAL_TIMER0, TIMER_MODE_NORMAL);
	timer_set_divider(PERIPHERAL_TIMER0, 3); // 16MHz/64 = 250KHz
	timer_set_prescaler(PERIPHERAL_TIMER0, 250 - 1); // 1-msec tick
	timer_set_reload(PERIPHERAL_TIMER0, 1000 - 1); // every 1-sec
	timer_enable_irq(PERIPHERAL_TIMER0, TIMER_EVENT_CC_0);

	irq_set_priority(IRQ_TIMER0, 3);
	irq_enable(IRQ_TIMER0);

	timer_start(PERIPHERAL_TIMER0);

	while (1) {
		/* hang */
	}

	return 0;
}

void ISR_TIMER0(void)
{
	timer_clear_event(PERIPHERAL_TIMER0, TIMER_EVENT_CC_0);

	static int led = 0;
	led ^= 1;
	gpio_write(LED_PIN, led);
}
