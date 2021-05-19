#ifndef ABOV_GPIO_LL_H
#define ABOV_GPIO_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/periph/gpio.h"

void gpio_ll_reset(periph_t port);
void gpio_ll_enable_port(periph_t port);
void gpio_ll_disable_port(periph_t port);
void gpio_ll_set_mode(periph_t port, uint32_t pin, gpio_mode_t mode);
void gpio_ll_set_altfunc(periph_t port, uint32_t pin, int altfunc);
void gpio_ll_set_speed(periph_t port, uint32_t pin, gpio_speed_t speed);
void gpio_ll_set_debouncer(periph_t port, uint32_t pin, uint32_t pclk_clocks);
void gpio_ll_enable_irq(periph_t port, uint32_t pin, gpio_irq_t irq_type);
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

#endif /* ABOV_GPIO_LL_H */
