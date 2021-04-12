#ifndef ABOV_I2C_LL_H
#define ABOV_I2C_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/peripheral.h"

typedef enum {
	I2C_EVENT_NONE				= 0,
	I2C_EVENT_STOP				= 1UL << 0,
	I2C_EVENT_BUSY				= 1UL << 1,
	I2C_EVENT_RX				= 1UL << 2,
	I2C_EVENT_TX				= 1UL << 3,
	I2C_EVENT_SLAVE				= 1UL << 4,
	I2C_EVENT_COLLISION			= 1UL << 5,
	I2C_EVENT_MASK				= (1UL << 6) - 1,
} i2c_event_t;

void i2c_reset(peripheral_t i2c);
void i2c_start(peripheral_t i2c);
void i2c_stop(peripheral_t i2c);
void i2c_set_slave_address(peripheral_t i2c, uint16_t slave_addr);
void i2c_enable_ack(peripheral_t i2c);
void i2c_disable_ack(peripheral_t i2c);
void i2c_enable_irq(peripheral_t i2c);
void i2c_disable_irq(peripheral_t i2c);
void i2c_write_byte(peripheral_t i2c, uint8_t value);
uint8_t i2c_read_byte(peripheral_t i2c);

i2c_event_t i2c_get_event(peripheral_t i2c);
void i2c_clear_event(peripheral_t i2c, i2c_event_t events);

void i2c_set_frequency(peripheral_t i2c, uint32_t hz, uint32_t pclk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_I2C_LL_H */
