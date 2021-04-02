#ifndef ABOV_GPIO_HAL_H
#define ABOV_GPIO_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/** Each port can have a number of pins up to `GPIO_PORT_SIZE`.
 * It should be kept power of 2. */
#define GPIO_PORT_SIZE			0x100U

/** GPIO port type */
typedef enum {
	/** */
	GPIOA				= 0,
	/** */
	GPIOB				= GPIOA + GPIO_PORT_SIZE,
	/** */
	GPIOC				= GPIOB + GPIO_PORT_SIZE,
	/** */
	GPIOD				= GPIOC + GPIO_PORT_SIZE,
	/** */
	GPIOE				= GPIOD + GPIO_PORT_SIZE,
	/** */
	GPIOF				= GPIOE + GPIO_PORT_SIZE,
} gpio_port_t;

/** GPIO mode */
typedef enum {
	/** Output only mode */
	GPIO_MODE_PUSHPULL,
	/** Input/output open-drain mode */
	GPIO_MODE_OPENDRAIN,
	/** Input only mode */
	GPIO_MODE_INPUT,
	/** Input only mode with pull-up function */
	GPIO_MODE_INPUT_PULLUP,
	/** Input only mode with pull-down function */
	GPIO_MODE_INPUT_PULLDOWN,
	/** Analog function */
	GPIO_MODE_ANALOG,
} gpio_mode_t;

/** GPIO interrupt type */
typedef enum {
	/** Interrupt trigger on rising edge */
	GPIO_INTR_EDGE_RISING,
	/** Interrupt trigger on falling edge */
	GPIO_INTR_EDGE_FALLING,
	/** Interrupt trigger on both rising and falling edges */
	GPIO_INTR_EDGE_ANY,
	/** Interrupt trigger on logic level high */
	GPIO_INTR_LEVEL_HIGH,
	/** Interrupt trigger on logic level low */
	GPIO_INTR_LEVEL_LOW,
} gpio_intr_t;

/**
 * Reset GPIO port
 *
 * This function makes GPIO port the reset state.
 */
void gpio_reset(gpio_port_t port);
/**
 * Initialize the given GPIO pin to the specified mode
 *
 * :param ngpio: GPIO number
 * :param mode: sets gpio operation mode
 * :return: true on success
 */
bool gpio_open(uint32_t ngpio, gpio_mode_t mode);
/**
 * Deinitialize the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :return: true on success
 */
bool gpio_close(uint32_t ngpio);
/**
 * Select GPIO alternate function
 *
 * :param ngpio: GPIO number
 * :param altfunc: alternate function number
 * :return: true on success
 */
bool gpio_set_altfunc(uint32_t ngpio, int altfunc);
/**
 * Write output level to the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :param value: logic output level. Only 0(low) and 1(high) are possible
 */
void gpio_write(uint32_t ngpio, int value);
/**
 * Read the current input level from the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :return: 0 when the logic level is low. 1 when the logic level is high
 */
int gpio_read(uint32_t ngpio);
/**
 * Write a value to the given GPIO port
 *
 * :param port: GPIO port
 * :param value: to be written to the GPIO port
 */
void gpio_write_port(gpio_port_t port, int value);
/**
 * Read the current value of the given GPIO port
 *
 * :param port: GPIO port
 * :return: the value read from the specified GPIO port
 */
int gpio_read_port(gpio_port_t port);
/**
 * Enable interrupt on the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :param intr_type: sets interrupt trigger type
 */
bool gpio_enable_intr(uint32_t ngpio, gpio_intr_t intr_type);
/**
 * Disable interrupt on the given GPIO pin
 *
 * :param ngpio: GPIO number
 */
bool gpio_disable_intr(uint32_t ngpio);
/**
 * Clear interru flag on the given GPIO pin
 *
 * :param ngpio: GPIO number
 */
void gpio_clear_intr_flag(uint32_t ngpio);
/**
 * Enable the given port
 *
 * :param port: GPIO port
 */
void gpio_enable_port(gpio_port_t port);
/**
 * Disable the given port
 *
 * :param port: GPIO port
 */
void gpio_disable_port(gpio_port_t port);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_GPIO_HAL_H */
