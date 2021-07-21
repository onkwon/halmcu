#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "halmcu/periph/i2c.h"
#include "a33g.h"

extern "C" {
static struct i2c_stub reg0, reg1;
struct i2c_stub * const I2C0 = &reg0;
struct i2c_stub * const I2C1 = &reg1;
}

TEST_GROUP(I2C) {
	void setup(void) {
		mock().ignoreOtherCalls();

		i2c_reset(PERIPH_I2C0);
		i2c_reset(PERIPH_I2C1);
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(I2C, reset_ShouldResetTheInterface) {
	LONGS_EQUAL(0, I2C0->CR);
	LONGS_EQUAL(0xFF, I2C0->DR);
	LONGS_EQUAL(0xFF, I2C0->SR);
	LONGS_EQUAL(0, I2C0->SAR);
	LONGS_EQUAL(0xFFFF, I2C0->SCLL);
	LONGS_EQUAL(0xFFFF, I2C0->SCLH);
	LONGS_EQUAL(0xFFFF, I2C0->SDH);
}

TEST(I2C, start_ShouldSetStartBitInCR) {
	i2c_start(PERIPH_I2C0);
	LONGS_EQUAL(1, I2C0->CR);
}

TEST(I2C, stop_ShouldSetStopBitInCR) {
	i2c_stop(PERIPH_I2C0);
	LONGS_EQUAL(2, I2C0->CR);
}

TEST(I2C, set_slave_address_ShouldWriteToSarRegister) {
	i2c_set_slave_address(PERIPH_I2C0, 0x5a);
	LONGS_EQUAL(0xb4, I2C0->SAR);
}

TEST(I2C, enable_ack_ShouldSetAckInCRRegister) {
	i2c_enable_ack(PERIPH_I2C0);
	LONGS_EQUAL(8, I2C0->CR);
}

TEST(I2C, disable_ack_ShouldClearAckInCRRegister) {
	i2c_enable_ack(PERIPH_I2C0);
	i2c_disable_ack(PERIPH_I2C0);
	LONGS_EQUAL(0, I2C0->CR);
}

TEST(I2C, enable_irq_ShouldSetIntBitInCrRegister) {
	i2c_enable_irq(PERIPH_I2C0);
	LONGS_EQUAL(0x10, I2C0->CR);
}

TEST(I2C, disable_irq_ShouldClearIntBitInCrRegister) {
	i2c_enable_irq(PERIPH_I2C0);
	i2c_disable_irq(PERIPH_I2C0);
	LONGS_EQUAL(0, I2C0->CR);
}

TEST(I2C, write_byte_ShouldWriteToDrRegister) {
	i2c_write_byte(PERIPH_I2C0, 0x5a);
	LONGS_EQUAL(0x5a, I2C0->DR);
}

TEST(I2C, read_byte_ShouldReturnDrValue) {
	I2C0->DR = 0xa5;
	LONGS_EQUAL(0xa5, i2c_read_byte(PERIPH_I2C0));
}

TEST(I2C, clear_event_ShouldWriteFfToSrRegister) {
	i2c_clear_event(PERIPH_I2C0, I2C_EVENT_MASK);
	LONGS_EQUAL(0xff, I2C0->SR);
}

TEST(I2C, get_event_ShouldReturnEvents) {
	I2C0->SR = 0xff;
	LONGS_EQUAL(I2C_EVENT_MASK, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x40;
	LONGS_EQUAL(I2C_EVENT_TX, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x20;
	LONGS_EQUAL(I2C_EVENT_STOP, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x10;
	LONGS_EQUAL(I2C_EVENT_SLAVE, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x08;
	LONGS_EQUAL(I2C_EVENT_COLLISION, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x04;
	LONGS_EQUAL(I2C_EVENT_BUSY, i2c_get_event(PERIPH_I2C0));
	I2C0->SR = 0x01;
	LONGS_EQUAL(I2C_EVENT_RX, i2c_get_event(PERIPH_I2C0));
}

TEST(I2C, set_frequency_ShouldSetSclFreqAndSdaHoldTime_When400KHzOn74MHz) {
	i2c_set_frequency(PERIPH_I2C0, 400000, 74000000);
	LONGS_EQUAL(92-1, I2C0->SCLL);
	LONGS_EQUAL(92-3, I2C0->SCLH);
	LONGS_EQUAL(22-4, I2C0->SDH);
}

TEST(I2C, set_frequency_ShouldSetSclFreqAndSdaHoldTime_When100KHzOn1MHz) {
	i2c_set_frequency(PERIPH_I2C0, 100000, 1000000);
	LONGS_EQUAL(5-1, I2C0->SCLL);
	LONGS_EQUAL(5-3, I2C0->SCLH);
	LONGS_EQUAL(0, I2C0->SDH);
}
