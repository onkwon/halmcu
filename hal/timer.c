#include "abov/hal/timer.h"

#include <stddef.h>

#include "abov/irq.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"

static void set_frequency(peripheral_t timer, const struct timer_cfg *cfg)
{
	uint32_t tclk = clk_get_frequency(clk_get_peripheral_clock_source(timer));
	uint32_t fclk = timer_get_frequency(timer, tclk);
	uint32_t tick = fclk / cfg->frequency;

	timer_set_prescaler(timer, tick - 1);
}

bool timer_init(peripheral_t timer, const struct timer_cfg *cfg)
{
	if (cfg == NULL) {
		return false;
	}

	pwr_enable_peripheral(timer);
	clk_enable_peripheral(timer);

	timer_reset(timer);

	if (cfg->set_clock_source != NULL) {
		cfg->set_clock_source();
	}

	timer_set_mode(timer, cfg->mode);
	if (cfg->mode != TIMER_MODE_CAPTURE) {
		set_frequency(timer, cfg);
	}

	if (cfg->irq != TIMER_EVENT_NONE) {
		timer_enable_irq(timer, cfg->irq);

		irq_set_priority(PERI_TO_IRQ(timer), cfg->irq_priority);
		irq_enable(PERI_TO_IRQ(timer));
	}

	return true;
}

void timer_deinit(peripheral_t timer)
{
	clk_disable_peripheral(timer);
	pwr_disable_peripheral(timer);
}
