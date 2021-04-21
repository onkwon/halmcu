#include "abov/hal/gpio.h"

#include <assert.h>
#include <stddef.h>

#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/irq.h"

void gpio_open(peripheral_t port, uint32_t pin, const struct gpio_cfg *cfg)
{
	if (cfg == NULL) {
		return;
	}

	pwr_enable_peripheral(port);
	clk_enable_peripheral(port);

	gpio_reset(port);
	gpio_enable_port(port);
	gpio_set_mode(port, pin, cfg->mode);

	if (cfg->altfunc) {
		gpio_set_altfunc(port, pin, cfg->altfunc_number);
	}

	gpio_set_speed(port, pin, cfg->speed);
	gpio_set_debouncer(port, pin, cfg->debounce);

	if (cfg->irq != 0) {
		gpio_clear_event(port, pin);
		gpio_enable_irq(port, pin, cfg->irq);
		irq_enable(PERI_TO_IRQ(port));
	}
}

void gpio_open_output(peripheral_t port, uint32_t pin, gpio_mode_t mode)
{
	pwr_enable_peripheral(port);
	clk_enable_peripheral(port);

	gpio_reset(port);
	gpio_enable_port(port);
	gpio_set_mode(port, pin, mode);
}

void gpio_close(peripheral_t port, uint32_t pin)
{
	gpio_disable_irq(port, pin);
	irq_disable(PERI_TO_IRQ(port));

	gpio_set_mode(port, pin, GPIO_MODE_ANALOG);
	gpio_set_debouncer(port, pin, 0); /* turn it off */
#if 0
	if (gpio_get_active_pins(port) == 0) {
		gpio_disable_port(port);
	}
#endif

	clk_disable_peripheral(port);
	pwr_disable_peripheral(port);
}
