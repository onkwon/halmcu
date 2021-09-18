#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "halmcu/periph/i2c.h"
#include "stm32f1.h"

extern "C" {
static I2C_Type i2c1_reg, i2c2_reg;
static RCC_Type rcc_reg;
I2C_Type * const I2C1 = &i2c1_reg;
I2C_Type * const I2C2 = &i2c2_reg;
RCC_Type * const RCC = &rcc_reg;
}

TEST_GROUP(I2C) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(I2C1, 0, sizeof(*I2C1));
		memset(I2C2, 0, sizeof(*I2C2));
		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(I2C, set_frequency_ShouldSetCcr_When100kHzOn8MHz) {
	i2c_set_frequency(PERIPH_I2C1, 100000, 8000000);
	LONGS_EQUAL(40, I2C1->CCR);
	LONGS_EQUAL(8+1, I2C1->TRISE);
}
TEST(I2C, set_frequency_ShouldSetCcrAndFs_When400kHzOn30MHz) {
	i2c_set_frequency(PERIPH_I2C1, 400000, 30000000);
	LONGS_EQUAL(25 | 0x8000, I2C1->CCR);
	LONGS_EQUAL(30+1, I2C1->TRISE);
}

TEST(I2C, enable_disable_ShouldSetOrClearPE) {
	i2c_enable(PERIPH_I2C1);
	LONGS_EQUAL(1, I2C1->CR1);
	i2c_disable(PERIPH_I2C1);
	LONGS_EQUAL(0, I2C1->CR1);
}

TEST(I2C, send_start_ShouldSetCr1) {
	i2c_send_start(PERIPH_I2C1);
	LONGS_EQUAL(1 << 8, I2C1->CR1);
}
TEST(I2C, send_stop_ShouldSetCr1) {
	i2c_send_stop(PERIPH_I2C1);
	LONGS_EQUAL(1 << 9, I2C1->CR1);
}

TEST(I2C, enable_disable_ack_ShouldSetOrClearACK) {
	i2c_enable_ack(PERIPH_I2C1);
	LONGS_EQUAL(1 << 10, I2C1->CR1);
	i2c_disable_ack(PERIPH_I2C1);
	LONGS_EQUAL(0, I2C1->CR1);
}

TEST(I2C, write_byte_ShouldWriteDR) {
	i2c_write_byte(PERIPH_I2C1, 0x5A);
	LONGS_EQUAL(0x5A, I2C1->DR);
}
TEST(I2C, read_byte_ShouldReturnDR) {
	I2C1->DR = 0xA5;
	LONGS_EQUAL(0xA5, i2c_read_byte(PERIPH_I2C1));
}
