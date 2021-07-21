#ifndef HALMCU_GPIO_LL_H
#define HALMCU_GPIO_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/gpio.h"

/**
 * @brief Reset GPIO port
 *
 * This function makes GPIO port the reset state.
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_ll_reset(periph_t port);
/**
 * @brief Enable the given port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_ll_enable_port(periph_t port);
/**
 * @brief Disable the given port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_ll_disable_port(periph_t port);
void gpio_ll_set_mode(periph_t port, uint32_t pin, gpio_mode_t mode);
void gpio_ll_set_altfunc(periph_t port, uint32_t pin, int altfunc);
/**
 * @note Call this function after @ref gpio_set_altfunc
 */
void gpio_ll_set_speed(periph_t port, uint32_t pin, gpio_speed_t speed);
/**
 * @param[in] pclk_clocks pass 0 clock cycle to disable
 */
void gpio_ll_set_debouncer(periph_t port, uint32_t pin, uint32_t pclk_clocks);
/**
 * @brief Enable interrupt on the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[in] irq_type sets interrupt trigger type
 */
void gpio_ll_enable_irq(periph_t port, uint32_t pin, gpio_irq_t irq_type);
/**
 * @brief Disable interrupt on the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 */
void gpio_ll_disable_irq(periph_t port, uint32_t pin);
void gpio_ll_clear_event(periph_t port, uint32_t pin);
/**
 * @brief Write a value to the given GPIO port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param value to be written to the GPIO port
 */
void gpio_ll_write_port(periph_t port, int value);
/**
 * @brief Read the current value of the given GPIO port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @return the value read from the specified GPIO port
 */
int gpio_ll_read_port(periph_t port);
void gpio_ll_write(periph_t port, uint32_t pin, int value);
int gpio_ll_read(periph_t port, uint32_t pin);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_GPIO_LL_H */
