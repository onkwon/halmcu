#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/gpio.h"
#include "stm32f1.h"
#include "assert.h"

extern "C" {
static RCC_Type rcc_reg;
static GPIO_Type rega, regb, regc, regd, rege, regf, regg;
RCC_Type * const RCC = &rcc_reg;
GPIO_Type * const GPIOA = &rega;
GPIO_Type * const GPIOB = &regb;
GPIO_Type * const GPIOC = &regc;
GPIO_Type * const GPIOD = &regd;
GPIO_Type * const GPIOE = &rege;
GPIO_Type * const GPIOF = &regf;
GPIO_Type * const GPIOG = &regg;
}

void assert_override(void) {
	mock().actualCall(__func__);
}

TEST_GROUP(GPIO) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(RCC, 0, sizeof(*RCC));
		memset(GPIOA, 0, sizeof(*GPIOA));
		memset(GPIOB, 0, sizeof(*GPIOB));
		memset(GPIOC, 0, sizeof(*GPIOC));
		memset(GPIOD, 0, sizeof(*GPIOD));
		memset(GPIOE, 0, sizeof(*GPIOE));
		memset(GPIOF, 0, sizeof(*GPIOF));
		memset(GPIOG, 0, sizeof(*GPIOG));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(GPIO, reset_ShouldSetRccRSTR) {
	gpio_reset(PERI_GPIOA);
	LONGS_EQUAL(4, RCC->APB2RSTR);
	gpio_reset(PERI_GPIOB);
	LONGS_EQUAL(0xc, RCC->APB2RSTR);
	gpio_reset(PERI_GPIOG);
	LONGS_EQUAL(0x10c, RCC->APB2RSTR);
}
TEST(GPIO, reset_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_reset(PERI_USART1);
}

TEST(GPIO, enable_port_ShouldSetRccENR) {
	gpio_enable_port(PERI_GPIOA);
	LONGS_EQUAL(0x4, RCC->APB2ENR);
	gpio_enable_port(PERI_GPIOB);
	LONGS_EQUAL(0xc, RCC->APB2ENR);
	gpio_enable_port(PERI_GPIOG);
	LONGS_EQUAL(0x10c, RCC->APB2ENR);
}
TEST(GPIO, disable_port_ShouldClearRccENR) {
	RCC->APB2ENR = 0xFFFF;
	gpio_disable_port(PERI_GPIOA);
	LONGS_EQUAL(0xfffb, RCC->APB2ENR);
	gpio_disable_port(PERI_GPIOB);
	LONGS_EQUAL(0xfff3, RCC->APB2ENR);
	gpio_disable_port(PERI_GPIOG);
	LONGS_EQUAL(0xfef3, RCC->APB2ENR);
}
TEST(GPIO, enable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_enable_port(PERI_USART1);
}
TEST(GPIO, disable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_disable_port(PERI_USART1);
}

TEST(GPIO, read_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_read_port(PERI_USART1);
}
TEST(GPIO, read_port_ShouldReturnIDR) {
	GPIOA->IDR = 0xA5A5;
	LONGS_EQUAL(0xA5A5, gpio_read_port(PERI_GPIOA));
	GPIOB->IDR = 0x5A5A;
	LONGS_EQUAL(0x5A5A, gpio_read_port(PERI_GPIOB));
}

TEST(GPIO, write_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_write_port(PERI_USART1, 0xabcd);
}
TEST(GPIO, write_port_ShouldSetODR) {
	gpio_write_port(PERI_GPIOA, 0xabcd);
	LONGS_EQUAL(0xabcd, GPIOA->ODR);
	gpio_write_port(PERI_GPIOB, 0xdcba);
	LONGS_EQUAL(0xdcba, GPIOB->ODR);
}

TEST(GPIO, read_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_read(PERI_USART1, 0);
}
TEST(GPIO, read_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("assert_override");
	gpio_read(PERI_GPIOA, 16);
}
TEST(GPIO, read_ShouldReturnIDRBit) {
	GPIOA->IDR = 0xA5;
	LONGS_EQUAL(1, gpio_read(PERI_GPIOA, 0));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOA, 1));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOA, 4));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOA, 7));
	GPIOB->IDR = 0x5A;
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 0));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 1));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 4));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 7));
}

TEST(GPIO, write_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_write(PERI_USART1, 0, 0);
}
TEST(GPIO, write_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("assert_override");
	gpio_write(PERI_GPIOA, 16, 0);
}
TEST(GPIO, write_ShouldSetBSRRBit) {
	gpio_write(PERI_GPIOA, 0, 1);
	LONGS_EQUAL(1, GPIOA->BSRR);
	gpio_write(PERI_GPIOA, 1, 0);
	LONGS_EQUAL(0x20000, GPIOA->BSRR);
	gpio_write(PERI_GPIOA, 15, 0);
	LONGS_EQUAL(0x80000000, GPIOA->BSRR);
	gpio_write(PERI_GPIOA, 14, 1);
	LONGS_EQUAL(0x4000, GPIOA->BSRR);
}
