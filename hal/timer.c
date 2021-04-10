#include "abov/hal/timer.h"

#include <stddef.h>

#include "abov/irq.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

static irq_t get_irq_from_peripheral(peripheral_t peri)
{
	switch (peri) {
	case PERIPHERAL_TIMER0:
		return IRQ_TIMER0;
	case PERIPHERAL_TIMER1:
		return IRQ_TIMER1;
	case PERIPHERAL_TIMER2:
		return IRQ_TIMER2;
	case PERIPHERAL_TIMER3:
		return IRQ_TIMER3;
	case PERIPHERAL_TIMER4:
		return IRQ_TIMER4;
	case PERIPHERAL_TIMER5:
		return IRQ_TIMER5;
	case PERIPHERAL_TIMER6:
		return IRQ_TIMER6;
	case PERIPHERAL_TIMER7:
		return IRQ_TIMER7;
	case PERIPHERAL_TIMER8:
		return IRQ_TIMER8;
	case PERIPHERAL_TIMER9:
		return IRQ_TIMER9;
	default:
		return IRQ_UNDEFINED;
	}
}

static void set_frequency(peripheral_t timer, const struct timer_cfg *cfg)
{
	uint32_t tclk = clk_get_frequency(clk_get_peripheral_clock_source(timer));
	uint32_t fclk = timer_get_frequency(timer, tclk);
	uint32_t tick = fclk / cfg->frequency;

	timer_set_prescaler(timer, tick - 1);
}

bool timer_init(peripheral_t timer, const struct timer_cfg *cfg)
{
	if (PERIPHERAL_GROUP(timer) != PERIPHERAL_TIMER) {
		return false;
	}
	if (cfg == NULL) {
		return false;
	}

	pwr_enable_peripheral(timer);
	clk_enable_peripheral(timer);

	timer_reset(timer);
	timer_set_mode(timer, cfg->mode);
	if (cfg->mode != TIMER_MODE_CAPTURE) {
		set_frequency(timer, cfg);
	}

	if (cfg->irq != TIMER_EVENT_NONE) {
		timer_enable_irq(timer, cfg->irq);

		irq_set_priority(get_irq_from_peripheral(timer), cfg->irq_priority);
		irq_enable(get_irq_from_peripheral(timer));
	}

	return true;
}

void timer_deinit(peripheral_t timer)
{
	if (PERIPHERAL_GROUP(timer) != PERIPHERAL_TIMER) {
		return;
	}

	clk_disable_peripheral(timer);
	pwr_disable_peripheral(timer);
}
