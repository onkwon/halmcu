#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/gpio.h"
#include "stm32f1.h"
#include "abov/assert.h"

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

TEST(GPIO, reset_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_reset(PERIPH_USART1);
}

TEST(GPIO, enable_port_ShouldSetRccENR) {
	gpio_enable_port(PERIPH_GPIOA);
	LONGS_EQUAL(0x4, RCC->APB2ENR);
	gpio_enable_port(PERIPH_GPIOB);
	LONGS_EQUAL(0xc, RCC->APB2ENR);
	gpio_enable_port(PERIPH_GPIOG);
	LONGS_EQUAL(0x10c, RCC->APB2ENR);
}
TEST(GPIO, disable_port_ShouldClearRccENR) {
	RCC->APB2ENR = 0xFFFF;
	gpio_disable_port(PERIPH_GPIOA);
	LONGS_EQUAL(0xfffb, RCC->APB2ENR);
	gpio_disable_port(PERIPH_GPIOB);
	LONGS_EQUAL(0xfff3, RCC->APB2ENR);
	gpio_disable_port(PERIPH_GPIOG);
	LONGS_EQUAL(0xfef3, RCC->APB2ENR);
}
TEST(GPIO, enable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_enable_port(PERIPH_USART1);
}
TEST(GPIO, disable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_disable_port(PERIPH_USART1);
}

TEST(GPIO, read_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_read_port(PERIPH_USART1);
}
TEST(GPIO, read_port_ShouldReturnIDR) {
	GPIOA->IDR = 0xA5A5;
	LONGS_EQUAL(0xA5A5, gpio_read_port(PERIPH_GPIOA));
	GPIOB->IDR = 0x5A5A;
	LONGS_EQUAL(0x5A5A, gpio_read_port(PERIPH_GPIOB));
}

TEST(GPIO, write_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_write_port(PERIPH_USART1, 0xabcd);
}
TEST(GPIO, write_port_ShouldSetODR) {
	gpio_write_port(PERIPH_GPIOC, 0xabcd);
	LONGS_EQUAL(0xabcd, GPIOC->ODR);
	gpio_write_port(PERIPH_GPIOD, 0xdcba);
	LONGS_EQUAL(0xdcba, GPIOD->ODR);
}

TEST(GPIO, read_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_read(PERIPH_USART1, 0);
}
TEST(GPIO, read_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_read(PERIPH_GPIOA, 16);
}
TEST(GPIO, read_ShouldReturnIDRBit) {
	GPIOA->IDR = 0xA5;
	LONGS_EQUAL(1, gpio_read(PERIPH_GPIOA, 0));
	LONGS_EQUAL(0, gpio_read(PERIPH_GPIOA, 1));
	LONGS_EQUAL(0, gpio_read(PERIPH_GPIOA, 4));
	LONGS_EQUAL(1, gpio_read(PERIPH_GPIOA, 7));
	GPIOB->IDR = 0x5A;
	LONGS_EQUAL(0, gpio_read(PERIPH_GPIOB, 0));
	LONGS_EQUAL(1, gpio_read(PERIPH_GPIOB, 1));
	LONGS_EQUAL(1, gpio_read(PERIPH_GPIOB, 4));
	LONGS_EQUAL(0, gpio_read(PERIPH_GPIOB, 7));
}

TEST(GPIO, write_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_write(PERIPH_USART1, 0, 0);
}
TEST(GPIO, write_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_write(PERIPH_GPIOA, 16, 0);
}
TEST(GPIO, write_ShouldSetBSRRBit) {
	gpio_write(PERIPH_GPIOE, 0, 1);
	LONGS_EQUAL(1, GPIOE->BSRR);
	gpio_write(PERIPH_GPIOE, 1, 0);
	LONGS_EQUAL(0x20000, GPIOE->BSRR);
	gpio_write(PERIPH_GPIOE, 15, 0);
	LONGS_EQUAL(0x80000000, GPIOE->BSRR);
	gpio_write(PERIPH_GPIOE, 14, 1);
	LONGS_EQUAL(0x4000, GPIOE->BSRR);
}

TEST(GPIO, enable_irq_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_enable_irq(PERIPH_USART1, 0, GPIO_IRQ_EDGE_ANY);
}
TEST(GPIO, enable_irq_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_enable_irq(PERIPH_GPIOA, 20, GPIO_IRQ_EDGE_ANY);
}
TEST(GPIO, enable_irq_ShouldResetExtiEdgeFirst) {
	mock().expectOneCall("exti_clear_rising_trigger")
		.withParameter("exti", 0);
	mock().expectOneCall("exti_clear_falling_trigger")
		.withParameter("exti", 0);
	gpio_enable_irq(PERIPH_GPIOA, 0, GPIO_IRQ_EDGE_ANY);
}
TEST(GPIO, enable_irq_ShouldCallExtiEnableIrq) {
	mock().expectOneCall("exti_enable_irq").withParameter("exti", 1);
	gpio_enable_irq(PERIPH_GPIOA, 1, GPIO_IRQ_EDGE_ANY);
}
TEST(GPIO, enable_irq_ShouldSetRisingEdge_WhenRisingOptionGiven) {
	mock().expectOneCall("exti_set_rising_trigger")
		.withParameter("exti", 2);
	mock().expectNoCall("exti_set_falling_trigger");
	gpio_enable_irq(PERIPH_GPIOA, 2, GPIO_IRQ_EDGE_RISING);
}
TEST(GPIO, enable_irq_ShouldSetFallingEdge_WhenFallingOptionGiven) {
	mock().expectOneCall("exti_set_falling_trigger")
		.withParameter("exti", 3);
	mock().expectNoCall("exti_set_rising_trigger");
	gpio_enable_irq(PERIPH_GPIOA, 3, GPIO_IRQ_EDGE_FALLING);
}
TEST(GPIO, enable_irq_ShouldSetBothEdges_WhenAnyOptionGiven) {
	mock().expectOneCall("exti_set_rising_trigger")
		.withParameter("exti", 4);
	mock().expectOneCall("exti_set_falling_trigger")
		.withParameter("exti", 4);
	gpio_enable_irq(PERIPH_GPIOA, 4, GPIO_IRQ_EDGE_ANY);
}

TEST(GPIO, disable_irq_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_disable_irq(PERIPH_USART1, 0);
}
TEST(GPIO, disable_irq_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_disable_irq(PERIPH_GPIOA, 20);
}
TEST(GPIO, disable_irq_ShouldCallExtiDisableIrq) {
	mock().expectOneCall("exti_disable_irq").withParameter("exti", 5);
	gpio_disable_irq(PERIPH_GPIOA, 5);
}

TEST(GPIO, clear_event_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_clear_event(PERIPH_USART1, 0);
}
TEST(GPIO, clear_event_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_clear_event(PERIPH_GPIOA, 20);
}
TEST(GPIO, clear_event_ShouldCallExtiClearEvent) {
	mock().expectOneCall("exti_clear_event").withParameter("exti", 6);
	gpio_clear_event(PERIPH_GPIOA, 6);
}

TEST(GPIO, set_mode_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_mode(PERIPH_USART1, 0, GPIO_MODE_PUSHPULL);
}
TEST(GPIO, set_mode_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_mode(PERIPH_GPIOA, 16, GPIO_MODE_PUSHPULL);
}
TEST(GPIO, set_mode_ShouldSetCRL_WhenPin0To7Given) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_INPUT);
	LONGS_EQUAL(0x4, GPIOA->CRL);
	gpio_set_mode(PERIPH_GPIOA, 1, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x24, GPIOA->CRL);
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_ANALOG);
	LONGS_EQUAL(0x20, GPIOA->CRL);
	gpio_set_mode(PERIPH_GPIOA, 2, GPIO_MODE_OPENDRAIN);
	LONGS_EQUAL(0x620, GPIOA->CRL);
	gpio_set_mode(PERIPH_GPIOA, 3, GPIO_MODE_INPUT_PULLUP);
	LONGS_EQUAL(0x8620, GPIOA->CRL);
}
TEST(GPIO, set_mode_ShouldSetCRH_WhenPin8To15Given) {
	gpio_set_mode(PERIPH_GPIOA, 15, GPIO_MODE_INPUT);
	LONGS_EQUAL(0x40000000, GPIOA->CRH);
	gpio_set_mode(PERIPH_GPIOA, 13, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x40200000, GPIOA->CRH);
}

TEST(GPIO, set_altfunc_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_altfunc(PERIPH_USART1, 0, 0);
}
TEST(GPIO, set_altfunc_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_altfunc(PERIPH_GPIOA, 20, 0);
}
TEST(GPIO, set_altfunc_ShouldSetCRL_WhenPin0To7AndPushpullGiven) {
	gpio_set_mode(PERIPH_GPIOG, 0, GPIO_MODE_PUSHPULL);
	gpio_set_altfunc(PERIPH_GPIOG, 0, 0);
	LONGS_EQUAL(0xa, GPIOG->CRL);
}
TEST(GPIO, set_altfunc_ShouldSetCRH_WhenPin8To15AndPushpullGiven) {
	gpio_set_mode(PERIPH_GPIOA, 15, GPIO_MODE_PUSHPULL);
	gpio_set_altfunc(PERIPH_GPIOA, 15, 0);
	LONGS_EQUAL(0xa0000000, GPIOA->CRH);
}
TEST(GPIO, set_altfunc_ShouldSetCR_WhenInputModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_INPUT);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(4, GPIOA->CRL);
}
TEST(GPIO, set_altfunc_ShouldSetCR_WhenInputPullupModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_INPUT_PULLUP);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(8, GPIOA->CRL);
}
TEST(GPIO, set_altfunc_ShouldSetCR_WhenInputPulldownModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_INPUT_PULLDOWN);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(8, GPIOA->CRL);
}
TEST(GPIO, set_altfunc_ShouldSetModeAltPushpull_WhenPushpullModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_PUSHPULL);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(0xa, GPIOA->CRL);
}
TEST(GPIO, set_altfunc_ShouldSetModeAltOpendrain_WhenOpendrainModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_OPENDRAIN);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(0xe, GPIOA->CRL);
}

TEST(GPIO, set_speed_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_speed(PERIPH_USART1, 0, GPIO_SPEED_LOW);
}
TEST(GPIO, set_speed_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("abov_assertion_failed");
	gpio_set_speed(PERIPH_GPIOA, 16, GPIO_SPEED_LOW);
}
TEST(GPIO, set_speed_ShouldSetSpeed) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_PUSHPULL);
	gpio_set_speed(PERIPH_GPIOA, 0, GPIO_SPEED_HIGH);
	LONGS_EQUAL(3, GPIOA->CRL);
	gpio_set_mode(PERIPH_GPIOA, 1, GPIO_MODE_PUSHPULL);
	gpio_set_speed(PERIPH_GPIOA, 1, GPIO_SPEED_MID);
	LONGS_EQUAL(0x13, GPIOA->CRL);
	gpio_set_speed(PERIPH_GPIOA, 0, GPIO_SPEED_LOW);
	LONGS_EQUAL(0x12, GPIOA->CRL);
}
TEST(GPIO, set_speed_ShouldSetSpeed_WhenPushpullModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_PUSHPULL);
	gpio_set_speed(PERIPH_GPIOA, 0, GPIO_SPEED_HIGH);
	LONGS_EQUAL(3, GPIOA->CRL);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(0xb, GPIOA->CRL);
}
TEST(GPIO, set_speed_ShouldSetSpeed_WhenOpendrainModeGiven) {
	gpio_set_mode(PERIPH_GPIOA, 0, GPIO_MODE_OPENDRAIN);
	gpio_set_speed(PERIPH_GPIOA, 0, GPIO_SPEED_MID);
	LONGS_EQUAL(5, GPIOA->CRL);
	gpio_set_altfunc(PERIPH_GPIOA, 0, 0);
	LONGS_EQUAL(0xd, GPIOA->CRL);
}
