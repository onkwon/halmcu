#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/ll/i2c.h"
#include "a33g.h"
#include "abov/peripheral.h"
#include "abov/delay.h"

extern "C" {
static struct i2c_stub reg0, reg1;
struct i2c_stub * const I2C0 = &reg0;
struct i2c_stub * const I2C1 = &reg1;
}

void udelay(unsigned int usec) {
	mock().actualCall(__func__);
	(void)usec;
}

TEST_GROUP(I2C) {
	void setup(void) {
		mock().ignoreOtherCalls();

		i2c_reset(PERI_I2C0);
		i2c_reset(PERI_I2C1);
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

TEST(I2C, set_slave_address_ShouldWriteToSarRegister) {
	i2c_set_slave_address(PERI_I2C0, 0x5a);
	LONGS_EQUAL(0xb4, I2C0->SAR);
}

TEST(I2C, enable_ack_ShouldSetAckInCRRegister) {
	i2c_enable_ack(PERI_I2C0);
	LONGS_EQUAL(8, I2C0->CR);
}

TEST(I2C, disable_ack_ShouldClearAckInCRRegister) {
	i2c_enable_ack(PERI_I2C0);
	i2c_disable_ack(PERI_I2C0);
	LONGS_EQUAL(0, I2C0->CR);
}

TEST(I2C, enable_irq_ShouldSetIntBitInCrRegister) {
	i2c_enable_irq(PERI_I2C0);
	LONGS_EQUAL(0x10, I2C0->CR);
}

TEST(I2C, disable_irq_ShouldClearIntBitInCrRegister) {
	i2c_enable_irq(PERI_I2C0);
	i2c_disable_irq(PERI_I2C0);
	LONGS_EQUAL(0, I2C0->CR);
}

TEST(I2C, set_frequency_ShouldSetSclFreqAndSdaHoldTime_When400KHzOn74MHz) {
	i2c_set_frequency(PERI_I2C0, 400000, 74000000);
	LONGS_EQUAL(92-1, I2C0->SCLL);
	LONGS_EQUAL(92-3, I2C0->SCLH);
	LONGS_EQUAL(22-4, I2C0->SDH);
}

TEST(I2C, set_frequency_ShouldSetSclFreqAndSdaHoldTime_When100KHzOn1MHz) {
	i2c_set_frequency(PERI_I2C0, 100000, 1000000);
	LONGS_EQUAL(5-1, I2C0->SCLL);
	LONGS_EQUAL(5-3, I2C0->SCLH);
	LONGS_EQUAL(0, I2C0->SDH);
}
