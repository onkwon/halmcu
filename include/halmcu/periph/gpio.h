#ifndef HALMCU_GPIO_H
#define HALMCU_GPIO_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "halmcu/periph/periph.h"

/** GPIO mode */
typedef enum {
	GPIO_MODE_PUSHPULL,		/**< Output only mode */
	GPIO_MODE_INPUT,		/**< Input only mode */
	GPIO_MODE_INPUT_PULLUP,		/**< Input only mode with pull-up function */
	GPIO_MODE_INPUT_PULLDOWN,	/**< Input only mode with pull-down function */
	GPIO_MODE_OPENDRAIN,		/**< Input/output open-drain mode */
	GPIO_MODE_OPENDRAIN_PULLUP,	/**< Input/output open-drain mode with pull-up function */
	GPIO_MODE_OPENDRAIN_PULLDOWN,	/**< Input/output open-drain mode with pull-down function */
	GPIO_MODE_ANALOG,		/**< Analog function */
} gpio_mode_t;

/** GPIO interrupt type */
typedef enum {
	GPIO_IRQ_NONE,
	GPIO_IRQ_EDGE_RISING,		/**< Rising edge interrupt */
	GPIO_IRQ_EDGE_FALLING,		/**< Falling edge interrupt */
	GPIO_IRQ_EDGE_ANY,		/**< Both rising and falling edge interrupts */
	GPIO_IRQ_LEVEL_HIGH,		/**< Logic level high interrupt */
	GPIO_IRQ_LEVEL_LOW,		/**< Logic level low interrupt */
} gpio_irq_t;

typedef enum {
	GPIO_SPEED_LOW,
	GPIO_SPEED_MID,
	GPIO_SPEED_HIGH,
} gpio_speed_t;

/**
 * @brief Reset GPIO port
 *
 * This function makes GPIO port the reset state.
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_reset(periph_t port);
/**
 * @brief Enable the given port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_enable_port(periph_t port);
/**
 * @brief Disable the given port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 */
void gpio_disable_port(periph_t port);
void gpio_set_mode(periph_t port, uint32_t pin, gpio_mode_t mode);
/**
 * @brief Select GPIO alternate function
 *
 * Call this function after :c:func:`gpio_set_mode`
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[in] altfunc alternate function number
 */
void gpio_set_altfunc(periph_t port, uint32_t pin, int altfunc);
/**
 * @note Call this function after @ref gpio_set_altfunc
 */
void gpio_set_speed(periph_t port, uint32_t pin, gpio_speed_t speed);
/**
 * @param[in] pclk_clocks pass 0 clock cycle to disable
 */
void gpio_set_debouncer(periph_t port, uint32_t pin, uint32_t pclk_clocks);
/**
 * @brief Enable interrupt on the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[in] irq_type sets interrupt trigger type
 */
void gpio_enable_irq(periph_t port, uint32_t pin, gpio_irq_t irq_type);
/**
 * @brief Disable interrupt on the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 */
void gpio_disable_irq(periph_t port, uint32_t pin);
/**
 * @brief Clear interrupt flag on the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param pin GPIO number starting from 0
 */
void gpio_clear_event(periph_t port, uint32_t pin);
/**
 * @brief Write output level to the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @param[out] value logic output level. Only 0(low) and 1(high) are possible
 */
void gpio_write_pin(periph_t port, uint32_t pin, int value);
/**
 * @brief Read the current input level from the given GPIO pin
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param[in] pin GPIO number starting from 0
 * @return 0 when the logic level is low. 1 when the logic level is high
 */
int gpio_read_pin(periph_t port, uint32_t pin);
/**
 * @brief Write a value to the given GPIO port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @param value to be written to the GPIO port
 */
void gpio_write_port(periph_t port, int value);
/**
 * @brief Read the current value of the given GPIO port
 *
 * @param[in] port GPIO port enumerated in @ref periph_t
 * @return the value read from the specified GPIO port
 */
int gpio_read_port(periph_t port);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_GPIO_H */
