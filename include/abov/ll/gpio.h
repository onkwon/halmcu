#ifndef ABOV_GPIO_HAL_H
#define ABOV_GPIO_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#if !defined(ABOV_PREFIX)
#define gpio_port_t				abov_gpio_port_t
#define gpio_mode_t				abov_gpio_mode_t
#define gpio_intr_t				abov_gpio_intr_t
#define gpio_reset				abov_gpio_reset
#define gpio_open				abov_gpio_open
#define gpio_close				abov_gpio_close
#define gpio_set_altfunc			abov_gpio_set_altfunc
#define gpio_write				abov_gpio_write
#define gpio_read				abov_gpio_read
#define gpio_write_port				abov_gpio_write_port
#define gpio_read_port				abov_gpio_read_port
#define gpio_enable_intr			abov_gpio_enable_intr
#define gpio_disable_intr			abov_gpio_disable_intr
#define gpio_clear_intr_flag			abov_gpio_clear_intr_flag
#define gpio_enable_port			abov_gpio_enable_port
#define gpio_disable_port			abov_gpio_disable_port
#endif

/** Each port can have a number of pins up to `ABOV_GPIO_PORT_SIZE`.
 * It should be kept power of 2. */
#define ABOV_GPIO_PORT_SIZE			0x100U

/** GPIO port type */
typedef enum {
	/** */
	ABOV_GPIOA				= 0,
	/** */
	ABOV_GPIOB				= ABOV_GPIOA + ABOV_GPIO_PORT_SIZE,
	/** */
	ABOV_GPIOC				= ABOV_GPIOB + ABOV_GPIO_PORT_SIZE,
	/** */
	ABOV_GPIOD				= ABOV_GPIOC + ABOV_GPIO_PORT_SIZE,
	/** */
	ABOV_GPIOE				= ABOV_GPIOD + ABOV_GPIO_PORT_SIZE,
	/** */
	ABOV_GPIOF				= ABOV_GPIOE + ABOV_GPIO_PORT_SIZE,
} abov_gpio_port_t;

/** GPIO mode */
typedef enum {
	/** Output only mode */
	ABOV_GPIO_MODE_PUSHPULL,
	/** Input/output open-drain mode */
	ABOV_GPIO_MODE_OPENDRAIN,
	/** Input only mode */
	ABOV_GPIO_MODE_INPUT,
	/** Input only mode with pull-up function */
	ABOV_GPIO_MODE_INPUT_PULLUP,
	/** Input only mode with pull-down function */
	ABOV_GPIO_MODE_INPUT_PULLDOWN,
	/** Analog function */
	ABOV_GPIO_MODE_ANALOG,
} abov_gpio_mode_t;

/** GPIO interrupt type */
typedef enum {
	/** Interrupt trigger on rising edge */
	ABOV_GPIO_INTR_EDGE_RISING,
	/** Interrupt trigger on falling edge */
	ABOV_GPIO_INTR_EDGE_FALLING,
	/** Interrupt trigger on both rising and falling edges */
	ABOV_GPIO_INTR_EDGE_ANY,
	/** Interrupt trigger on logic level high */
	ABOV_GPIO_INTR_LEVEL_HIGH,
	/** Interrupt trigger on logic level low */
	ABOV_GPIO_INTR_LEVEL_LOW,
} abov_gpio_intr_t;

/**
 * Reset GPIO port
 *
 * This function makes GPIO port the reset state.
 */
void abov_gpio_reset(abov_gpio_port_t port);
/**
 * Initialize the given GPIO pin to the specified mode
 *
 * :param ngpio: GPIO number
 * :param mode: sets gpio operation mode
 * :return: true on success
 */
bool abov_gpio_open(uint32_t ngpio, abov_gpio_mode_t mode);
/**
 * Deinitialize the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :return: true on success
 */
bool abov_gpio_close(uint32_t ngpio);
/**
 * Select GPIO alternate function
 *
 * :param ngpio: GPIO number
 * :param altfunc: alternate function number
 * :return: true on success
 */
bool abov_gpio_set_altfunc(uint32_t ngpio, int altfunc);
/**
 * Write output level to the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :param value: logic output level. Only 0(low) and 1(high) are possible
 */
void abov_gpio_write(uint32_t ngpio, int value);
/**
 * Read the current input level from the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :return: 0 when the logic level is low. 1 when the logic level is high
 */
int abov_gpio_read(uint32_t ngpio);
/**
 * Write a value to the given GPIO port
 *
 * :param port: GPIO port
 * :param value: to be written to the GPIO port
 */
void abov_gpio_write_port(abov_gpio_port_t port, int value);
/**
 * Read the current value of the given GPIO port
 *
 * :param port: GPIO port
 * :return: the value read from the specified GPIO port
 */
int abov_gpio_read_port(abov_gpio_port_t port);
/**
 * Enable interrupt on the given GPIO pin
 *
 * :param ngpio: GPIO number
 * :param intr_type: sets interrupt trigger type
 */
bool abov_gpio_enable_intr(uint32_t ngpio, abov_gpio_intr_t intr_type);
/**
 * Disable interrupt on the given GPIO pin
 *
 * :param ngpio: GPIO number
 */
bool abov_gpio_disable_intr(uint32_t ngpio);
/**
 * Clear interru flag on the given GPIO pin
 *
 * :param ngpio: GPIO number
 */
void abov_gpio_clear_intr_flag(uint32_t ngpio);
/**
 * Enable the given port
 *
 * :param port: GPIO port
 */
void abov_gpio_enable_port(abov_gpio_port_t port);
/**
 * Disable the given port
 *
 * :param port: GPIO port
 */
void abov_gpio_disable_port(abov_gpio_port_t port);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_GPIO_HAL_H */
