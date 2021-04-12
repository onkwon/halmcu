#ifndef ABOV_I2C_LL_H
#define ABOV_I2C_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/peripheral.h"

void i2c_reset(peripheral_t i2c);
void i2c_enable_ack(peripheral_t i2c);
void i2c_disable_ack(peripheral_t i2c);
void i2c_enable_irq(peripheral_t i2c);
void i2c_disable_irq(peripheral_t i2c);
void i2c_set_slave_address(peripheral_t i2c, uint16_t slave_addr);

void i2c_set_frequency(peripheral_t i2c, uint32_t hz, uint32_t pclk);

bool i2c_start(peripheral_t i2c, uint16_t slave_address, bool readonly);
void i2c_stop(peripheral_t i2c);
bool i2c_write_byte(peripheral_t i2c, uint8_t value);
int i2c_read_byte(peripheral_t i2c, bool ack);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_I2C_LL_H */
