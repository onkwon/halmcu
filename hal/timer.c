#include "halmcu/hal/timer.h"

#include <stddef.h>

#include "halmcu/irq.h"
#include "halmcu/periph/pwr.h"
#include "halmcu/periph/clk.h"

static void set_frequency(periph_t timer, const struct timer_cfg *cfg)
{
	uint32_t tclk = clk_get_peripheral_clock_source_frequency(timer);
	uint32_t fclk = timer_get_frequency(timer, tclk);
	uint32_t tick = fclk / cfg->frequency;

	timer_set_prescaler(timer, tick - 1);
}

bool timer_init(periph_t timer, const struct timer_cfg *cfg)
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

		irq_set_priority(PERIPH_TO_IRQ(timer), cfg->irq_priority);
		irq_enable(PERIPH_TO_IRQ(timer));
	}

	return true;
}

void timer_deinit(periph_t timer)
{
	clk_disable_peripheral(timer);
	pwr_disable_peripheral(timer);
}
