#include "abov/ll/i2c.h"
#include <stddef.h>
#include "abov/bitop.h"
#include "abov/delay.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"

#define KHZ					1000U
#define MHZ					(KHZ * KHZ)
#define SDA_HOLD_TIME_NS			300U

#define SOFTRESET				(1U << 5)

typedef enum {
	I2C_FSM_WAITING,
	I2C_FSM_STOP,
	I2C_FSM_START_SEND,
	I2C_FSM_START_RECV,
	I2C_FSM_SENDING,
	I2C_FSM_RECEIVING,
	I2C_FSM_ERROR,
} i2c_fsm_t;

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

static void clear_event(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->SR = 0xffU;
}

static uint32_t get_event(peripheral_t i2c)
{
	return get_regs_from_peri(i2c)->SR;
}

static void set_start(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	bitop_set(&regs->CR, 0); /* START */
}

static void set_stop(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	bitop_set(&regs->CR, 1); /* STOP */
}

static void clear_stop(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	bitop_clear(&regs->CR, 1); /* STOP */
}

static void set_txd(peripheral_t i2c, uint8_t value)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->DR = (uint32_t)value;
}

static uint8_t get_rxd(peripheral_t i2c)
{
	return (uint8_t)get_regs_from_peri(i2c)->DR;
}

static void enable_ack(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	bitop_set(&regs->CR, 3); /* ACKEN */
}

static void disable_ack(peripheral_t i2c)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	bitop_clear(&regs->CR, 3); /* ACKEN */
}

static i2c_fsm_t process_gcall(uint32_t event)
{
	if (!(event & 0x01)) { /* ACK */
		return I2C_FSM_ERROR;
	} else if (event & 0x02) { /* TMODE */
		return I2C_FSM_START_SEND;
	}
	return I2C_FSM_START_RECV;
}

static i2c_fsm_t process_xfer(uint32_t event)
{
	if (event & 0x02) { /* TMODE */
		if (!(event & 0x01)) { /* ACK */
			return I2C_FSM_ERROR;
		}
		return I2C_FSM_SENDING;
	}
	return I2C_FSM_RECEIVING;
}

static i2c_fsm_t run_fsm(uint32_t event)
{
	if ((event & ~6U) == 0) {
		return I2C_FSM_WAITING;
	} else if (event & 0x20) { /* STOP */
		return I2C_FSM_STOP;
	} else if (event & 0x08) { /* MLOST */
		return I2C_FSM_ERROR;
	} else if (event & 0x80) { /* GCALL */
		return process_gcall(event);
	} else if (event & 0x40) { /* TEND */
		return process_xfer(event);
	}
	return I2C_FSM_ERROR;
}

static i2c_fsm_t poll_event(peripheral_t i2c)
{
	i2c_fsm_t rc;

	while ((rc = run_fsm(get_event(i2c))) == I2C_FSM_WAITING) {
		/* TODO: figure out why it doesn't work without delay? */
		udelay(10);
	}

	return rc;
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

void i2c_set_slave_address(peripheral_t i2c, uint16_t slave_address)
{
	I2C_Type *regs = get_regs_from_peri(i2c);
	regs->SAR = (slave_address << 1) & 0xfe;
}

void i2c_enable_ack(peripheral_t i2c)
{
	enable_ack(i2c);
}

void i2c_disable_ack(peripheral_t i2c)
{
	disable_ack(i2c);
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

bool i2c_start(peripheral_t i2c, uint16_t slave_address, bool readonly)
{
	// TODO: support 10-bit slave address
	set_txd(i2c, (uint8_t)((slave_address << 1) | readonly));
	set_start(i2c);

	i2c_fsm_t rc = poll_event(i2c);

	if (rc == I2C_FSM_START_RECV || rc == I2C_FSM_START_SEND) {
		return true;
	} else if (rc == I2C_FSM_SENDING) {
		clear_event(i2c);
		return poll_event(i2c) == I2C_FSM_START_RECV;
	}

	i2c_stop(i2c);
	return false;
}

void i2c_stop(peripheral_t i2c)
{
	set_stop(i2c);
	while (!(get_event(i2c) & 0x20)) { /* STOP */
		clear_event(i2c);
		poll_event(i2c);
	}
	clear_stop(i2c);
}

bool i2c_write_byte(peripheral_t i2c, uint8_t value)
{
	udelay(200);
	set_txd(i2c, value);
	clear_event(i2c);

	i2c_fsm_t rc = poll_event(i2c);
	if (rc == I2C_FSM_SENDING) {
		return true;
	}

	i2c_stop(i2c);
	return false;
}

int i2c_read_byte(peripheral_t i2c, bool ack)
{
	if (ack) {
		enable_ack(i2c);
	} else {
		disable_ack(i2c);
	}

	clear_event(i2c);
	i2c_fsm_t rc = poll_event(i2c);

	if (rc != I2C_FSM_RECEIVING) {
		i2c_stop(i2c);
		return -1;
	}

	return (int)get_rxd(i2c);
}
