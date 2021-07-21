#ifndef HALMCU_GPIO_HAL_H
#define HALMCU_GPIO_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/ll/gpio.h"
#include <stdbool.h>

/** GPIO configuration */
struct gpio_cfg {
	gpio_mode_t mode;
	gpio_irq_t irq;
	gpio_speed_t speed;
	bool altfunc;
	int altfunc_number;
	uint32_t debounce;
};

/**
 * @brief Initialize the given GPIO pin to the specified mode
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[in] cfg configuration
 *
 * @return true on success
 */
void gpio_open(periph_t port, uint32_t pin, const struct gpio_cfg *cfg);
/**
 * @brief Initialize the given GPIO pin to the specified mode
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[in] mode sets gpio operation mode
 * @return true on success
 */
void gpio_open_output(periph_t port, uint32_t pin, gpio_mode_t mode);
/**
 * @brief Deinitialize the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @return true on success
 */
void gpio_close(periph_t port, uint32_t pin);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_GPIO_HAL_H */
