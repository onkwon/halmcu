#include "halmcu/hal/gpio.h"

#include <stddef.h>

#include "halmcu/ll/pwr.h"
#include "halmcu/ll/clk.h"
#include "halmcu/irq.h"

void gpio_open(periph_t port, uint32_t pin, const struct gpio_cfg *cfg)
{
	if (cfg == NULL) {
		return;
	}

	pwr_ll_enable_peripheral(port);
	clk_ll_enable_peripheral(port);

	gpio_ll_enable_port(port);
	gpio_ll_set_mode(port, pin, cfg->mode);

	if (cfg->altfunc) {
		gpio_ll_set_altfunc(port, pin, cfg->altfunc_number);
	}

	gpio_ll_set_speed(port, pin, cfg->speed);
	gpio_ll_set_debouncer(port, pin, cfg->debounce);

	if (cfg->irq != 0) {
		gpio_ll_clear_event(port, pin);
		gpio_ll_enable_irq(port, pin, cfg->irq);
		irq_enable(PERIPH_TO_IRQ(port));
	}
}

void gpio_open_output(periph_t port, uint32_t pin, gpio_mode_t mode)
{
	pwr_ll_enable_peripheral(port);
	clk_ll_enable_peripheral(port);

	gpio_ll_enable_port(port);
	gpio_ll_set_mode(port, pin, mode);
}

void gpio_close(periph_t port, uint32_t pin)
{
	gpio_ll_disable_irq(port, pin);
	irq_disable(PERIPH_TO_IRQ(port));

	gpio_ll_set_mode(port, pin, GPIO_MODE_ANALOG);
	gpio_ll_set_debouncer(port, pin, 0); /* turn it off */
#if 0
	if (gpio_ll_get_active_pins(port) == 0) {
		gpio_ll_reset(port);
		gpio_ll_disable_port(port);
	}
#endif

	clk_ll_disable_peripheral(port);
	pwr_ll_disable_peripheral(port);
}

void gpio_write(periph_t port, uint32_t pin, int value)
{
	gpio_ll_write(port, pin, value);
}

int gpio_read(periph_t port, uint32_t pin)
{
	return gpio_ll_read(port, pin);
}

void gpio_set_altfunc(periph_t port, uint32_t pin, int altfunc)
{
	gpio_ll_set_altfunc(port, pin, altfunc);
}
