#include "halmcu/periph/i2c.h"
#include <stddef.h>
#include "halmcu/bitop.h"
#include "halmcu/asm/arm/cmsis.h"
#include "a33g.h"

#define KHZ					1000U
#define MHZ					(KHZ * KHZ)
#define SDA_HOLD_TIME_NS			300U

#define SOFTRESET				(1U << 5)

static I2C_Type *get_regs_from_peri(periph_t peri)
{
	switch (peri) {
	case PERIPH_I2C0:
		return I2C0;
	case PERIPH_I2C1:
		return I2C1;
	default:
		return NULL;
	}
}

void i2c_reset(periph_t i2c)
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

void i2c_start(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 0); /* START */
}

void i2c_stop(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 1); /* STOP */
}

void i2c_set_slave_address(periph_t i2c, uint16_t slave_addr)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	regs->SAR = (slave_addr << 1) & 0xfe;
}

void i2c_enable_ack(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 3); /* ACKEN */
}

void i2c_disable_ack(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_clear(&regs->CR, 3); /* ACKEN */
}

void i2c_enable_irq(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_set(&regs->CR, 4); /* IINTEN */
}

void i2c_disable_irq(periph_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);

	bitop_clear(&regs->CR, 4); /* IINTEN */
}

void i2c_write_byte(periph_t i2c, uint8_t value)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->DR = (uint32_t)value;
}

uint8_t i2c_read_byte(periph_t i2c)
{
	const I2C_Type *regs = get_regs_from_peri(i2c);
	return (uint8_t)regs->DR;
}

void i2c_clear_event(periph_t i2c, i2c_event_t events)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->SR = 0xffU | events;
}

i2c_event_t i2c_get_event(periph_t i2c)
{
	const I2C_Type *regs = get_regs_from_peri(i2c);
	uint32_t status = regs->SR;
	i2c_event_t event = I2C_EVENT_NONE;

	if (status & 0x40) { /* TEND */
		event = (i2c_event_t)(event | I2C_EVENT_TX);
	}
	if (status & 0x20) { /* STOP */
		event = (i2c_event_t)(event | I2C_EVENT_STOP);
	}
	if (status & 0x10) { /* SSEL */
		event = (i2c_event_t)(event | I2C_EVENT_SLAVE);
	}
	if (status & 0x08) { /* MLOST */
		event = (i2c_event_t)(event | I2C_EVENT_COLLISION);
	}
	if (status & 0x04) { /* BUSY */
		event = (i2c_event_t)(event | I2C_EVENT_BUSY);
	}
	if (status & 0x01) { /* RXACK */
		event = (i2c_event_t)(event | I2C_EVENT_RX);
	}

	return event;
}

void i2c_set_frequency(periph_t i2c, uint32_t hz, uint32_t pclk)
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
