#include "abov/hal/gpio.h"
#include "abov/hal/timer.h"
#include "abov/irq.h"

#define MHZ					1000000U
#define TIMER_SOURCE_CLOCK_MHZ			16U
#define TIMER_COUNTER_WIDTH			(1U << 16)

static volatile uint32_t measured_hz;

static void target_timer_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_PUSHPULL,
		.altfunc = true,
		.altfunc_number = 1,
	};
	gpio_open(PERIPH_GPIOB, 0, &cfg);
}

static void capture_timer_gpio_init(void)
{
	struct gpio_cfg cfg = {
		.mode = GPIO_MODE_INPUT,
		.altfunc = true,
		.altfunc_number = 1,
	};
	gpio_open(PERIPH_GPIOA, 7, &cfg);
}

static void set_target_timer_clock_source(void)
{
	timer_ll_set_clock_divider(PERIPH_TIMER0, 3); // 16MHz/64 = 250KHz
}

static void target_timer_init(void)
{
	target_timer_gpio_init();

	timer_init(PERIPH_TIMER0, &(struct timer_cfg) {
			.mode = TIMER_MODE_NORMAL,
			.frequency = 1000,
			.set_clock_source = set_target_timer_clock_source, });
	timer_set_reload(PERIPH_TIMER0, 1000 - 1); // every 1-sec

	timer_start(PERIPH_TIMER0);
}

static void capture_timer_init(void)
{
	capture_timer_gpio_init();

	timer_init(PERIPH_TIMER1, &(struct timer_cfg) {
			.mode = TIMER_MODE_CAPTURE,
			.irq = (timer_event_t)(TIMER_EVENT_CC_0 |
					TIMER_EVENT_CC_1 |
					TIMER_EVENT_OVERFLOW),
			.irq_priority = 3, });
	timer_set_edge(PERIPH_TIMER1, TIMER_RISING_EDGE);

	timer_start(PERIPH_TIMER1);
}

int main(void)
{
	target_timer_init();
	capture_timer_init();

	while (1) {
		uint32_t hz = measured_hz / MHZ;
		uint32_t dec = measured_hz % MHZ;
		(void)hz;
		(void)dec;
	}

	return 0;
}

void ISR_TIMER1(void)
{
	static uint32_t ovf = 0;
	timer_event_t event = timer_get_event(PERIPH_TIMER1);

	if (event & TIMER_EVENT_OVERFLOW) {
		ovf++;
	}
	if (event & TIMER_EVENT_CC_0) {
		uint32_t captured1 =
			TIMER_COUNTER_WIDTH - timer_get_cc(PERIPH_TIMER1, 0);
		uint32_t captured2 = timer_get_cc(PERIPH_TIMER1, 1) + 1;
		uint32_t ticks =
			ovf * TIMER_COUNTER_WIDTH + captured1 + captured2;
		uint32_t hz = ticks / TIMER_SOURCE_CLOCK_MHZ;

		measured_hz = hz;
		ovf = 0;
	}
	if (event & TIMER_EVENT_CC_1) {
	}

	timer_clear_event(PERIPH_TIMER1, (timer_event_t)
			(TIMER_EVENT_OVERFLOW |
			 TIMER_EVENT_CC_0 |
			 TIMER_EVENT_CC_1));
}
