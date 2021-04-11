#include "abov/ll/i2c.h"
#include <stddef.h>
#include "abov/bitop.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define KHZ					1000UL
#define MHZ					(KHZ * KHZ)
#define SDA_HOLD_TIME_NS			300

#define SOFTRESET				(1UL << 5)

static I2C_Type *get_regs_from_peri(peripheral_t peri)
{
	switch (peri) {
	case PERI_I2C0:
		return I2C0;
	case PERI_I2C1:
		return I2C1;
	default:
		return NULL;
	}
}

void i2c_reset(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	regs->CR |= SOFTRESET;
	regs->DR = 0xFF;
	regs->SR = 0xFF;
	regs->SAR = 0;
	regs->SCLL = 0xFFFF;
	regs->SCLH = 0xFFFF;
	regs->SDH = 0xFFFF;
	regs->CR = 0;
}

void i2c_start(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 0); /* START */
}

void i2c_stop(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 1); /* STOP */
}

void i2c_set_slave_address(peripheral_t i2c, uint16_t slave_addr)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	regs->SAR = (slave_addr << 1) & 0xfe;
}

void i2c_enable_ack(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 3); /* ACKEN */
}

void i2c_disable_ack(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_clear(&regs->CR, 3); /* ACKEN */
}

void i2c_enable_irq(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 4); /* IINTEN */
}

void i2c_disable_irq(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_clear(&regs->CR, 4); /* IINTEN */
}

void i2c_write_byte(peripheral_t i2c, uint8_t value)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->DR = (uint32_t)value;
}

uint8_t i2c_read_byte(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	return (uint8_t)regs->DR;
}

void i2c_clear_event(peripheral_t i2c, i2c_event_t events)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->SR = 0xffUL | events;
}

i2c_event_t i2c_get_event(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	uint32_t status = regs->SR;
	i2c_event_t event = I2C_EVENT_NONE;

	if (status & 0x40) { /* TEND */
		event |= I2C_EVENT_TX;
	}
	if (status & 0x20) { /* STOP */
		event |= I2C_EVENT_STOP;
	}
	if (status & 0x10) { /* SSEL */
		event |= I2C_EVENT_SLAVE;
	}
	if (status & 0x08) { /* MLOST */
		event |= I2C_EVENT_COLLISION;
	}
	if (status & 0x04) { /* BUSY */
		event |= I2C_EVENT_BUSY;
	}
	if (status & 0x01) { /* RXACK */
		event |= I2C_EVENT_RX;
	}

	return event;
}

void i2c_set_frequency(peripheral_t i2c, uint32_t hz, uint32_t pclk)
{
	uint32_t khz = hz / KHZ;
	uint32_t ksec = MHZ / khz / 2;
	uint32_t scl = ksec * (pclk / MHZ) / KHZ;
	uint32_t sdh = SDA_HOLD_TIME_NS * (pclk / MHZ) / KHZ;

	I2C_Type *regs = get_regs_from_peri(i2c);

	regs->SCLL = scl - 1;
	regs->SCLH = scl - 3;
	regs->SDH = (sdh < 4)? 0 : sdh - 4;
}
