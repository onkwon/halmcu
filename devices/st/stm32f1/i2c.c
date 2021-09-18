#include "halmcu/periph/i2c.h"

#include <stddef.h>

#include "halmcu/assert.h"
#include "halmcu/bitop.h"
#include "halmcu/compiler.h"
#include "stm32f1.h"

#define MHZ					1000000U

static I2C_Type *get_reg_from_peri(periph_t peri)
{
	switch (peri) {
	case PERIPH_I2C1:
		return I2C1;
	case PERIPH_I2C2:
		return I2C2;
	default:
		assert(0);
		return NULL;
	}
}

static bool is_busy(periph_t i2c)
{
	return get_reg_from_peri(i2c)->SR2 & (1U << 1/*BUSY*/);
}

void i2c_reset(periph_t i2c)
{
	uint32_t pos = 21; /* i2c1 */
	volatile uint32_t *reg = &RCC->APB1RSTR;

	if (i2c == PERIPH_I2C2) {
		pos = 22;
	}

	bitop_set(reg, pos);
	bitop_clear(reg, pos);
}

void i2c_set_frequency(periph_t i2c, uint32_t hz, uint32_t pclk)
{
	assert(pclk > hz);

	I2C_Type *reg = get_reg_from_peri(i2c);
	uint32_t freq = pclk / MHZ;
	uint32_t ccr = pclk / hz / 2;

	assert(freq >= 2 && freq <= 50);

	bitop_clean_set_with_mask(&reg->CR2, 0/*FREQ*/, 0x3F, freq);
	/* fast mode when faster than 100kHz */
	if (hz > 100000) {
		bitop_set(&reg->CCR, 15/*F/S*/);
		ccr = pclk / hz / 3;
	} else {
		bitop_clear(&reg->CCR, 15/*F/S*/);
	}
	bitop_clean_set_with_mask(&reg->CCR, 0/*CCR*/, 0xFFF, ccr);
	reg->TRISE = pclk / MHZ + 1;
}

void i2c_enable(periph_t i2c)
{
	bitop_set(&get_reg_from_peri(i2c)->CR1, 0);
}

void i2c_disable(periph_t i2c)
{
	bitop_clear(&get_reg_from_peri(i2c)->CR1, 0);
}

void i2c_send_start(periph_t i2c)
{
	bitop_set(&get_reg_from_peri(i2c)->CR1, 8);
}

void i2c_send_stop(periph_t i2c)
{
	bitop_set(&get_reg_from_peri(i2c)->CR1, 9);
}

void i2c_enable_ack(periph_t i2c)
{
	bitop_set(&get_reg_from_peri(i2c)->CR1, 10);
}

void i2c_disable_ack(periph_t i2c)
{
	bitop_clear(&get_reg_from_peri(i2c)->CR1, 10);
}

void i2c_write_byte(periph_t i2c, uint8_t value)
{
	get_reg_from_peri(i2c)->DR = value;
}

uint8_t i2c_read_byte(periph_t i2c)
{
	return (uint8_t)get_reg_from_peri(i2c)->DR;
}

bool i2c_is_busy(periph_t i2c)
{
	return is_busy(i2c);
}

bool i2c_has_started(periph_t i2c)
{
	return get_reg_from_peri(i2c)->SR1 & (1U << 0/*SB*/);
}

bool i2c_has_address_set(periph_t i2c)
{
	bool res = get_reg_from_peri(i2c)->SR1 & (1U << 1/*ADDR*/);
	unused(get_reg_from_peri(i2c)->SR2); /* clear SR1.ADDR bit */
	return res;
}

bool i2c_has_transfer_completed(periph_t i2c)
{
	return get_reg_from_peri(i2c)->SR1 & (1U << 2/*BTF*/);
}

bool i2c_has_received(periph_t i2c)
{
	return get_reg_from_peri(i2c)->SR1 & (1U << 6/*RxNE*/);
}
