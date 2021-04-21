#ifndef ABOV_GPIO_HAL_H
#define ABOV_GPIO_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/gpio.h"
#include <stdbool.h>

/** GPIO configuration */
struct gpio_cfg {
	/***/
	gpio_mode_t mode;
	/***/
	gpio_irq_t irq;
	/***/
	gpio_speed_t speed;
	/***/
	bool altfunc;
	/***/
	int altfunc_number;
	/***/
	uint32_t debounce;
};

/**
 * Initialize the given GPIO pin to the specified mode
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param cfg: configuration
 * :return: true on success
 */
void gpio_open(peripheral_t port, uint32_t pin, const struct gpio_cfg *cfg);
/**
 * Initialize the given GPIO pin to the specified mode
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :param mode: sets gpio operation mode
 * :return: true on success
 */
void gpio_open_output(peripheral_t port, uint32_t pin, gpio_mode_t mode);
/**
 * Deinitialize the given GPIO pin
 *
 * :param port: GPIO port enumerated in :c:type:`peripheral_t`
 * :param pin: GPIO number starting from 0
 * :return: true on success
 */
void gpio_close(peripheral_t port, uint32_t pin);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_GPIO_HAL_H */
