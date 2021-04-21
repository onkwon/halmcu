#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "../devices/st/stm32f1/exti.h"
#include "stm32f1.h"
#include "abov/ll/gpio.h"

extern "C" {
static SYSCFG_Type syscfg_reg;
static EXTI_Type exti_reg;
SYSCFG_Type * const SYSCFG = &syscfg_reg;
EXTI_Type * const EXTI = &exti_reg;
}

TEST_GROUP(EXTI) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(SYSCFG, 0, sizeof(*SYSCFG));
		memset(EXTI, 0, sizeof(*EXTI));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(EXTI, enable_irq_ShouldSetIMR) {
	exti_enable_irq(EXTI_0);
	LONGS_EQUAL(1, EXTI->IMR);
	exti_enable_irq(EXTI_1);
	LONGS_EQUAL(3, EXTI->IMR);
	exti_enable_irq(EXTI_5);
	LONGS_EQUAL(0x23, EXTI->IMR);
	exti_enable_irq(EXTI_8);
	LONGS_EQUAL(0x123, EXTI->IMR);
	exti_enable_irq(EXTI_19);
	LONGS_EQUAL(0x80123, EXTI->IMR);
	exti_enable_irq(EXTI_14);
	LONGS_EQUAL(0x84123, EXTI->IMR);
}
TEST(EXTI, disable_irq_ShouldClearIMR) {
	EXTI->IMR = 0xFFFFF;
	exti_disable_irq(EXTI_19);
	LONGS_EQUAL(0x7FFFF, EXTI->IMR);
	exti_disable_irq(EXTI_14);
	LONGS_EQUAL(0x7BFFF, EXTI->IMR);
	exti_disable_irq(EXTI_9);
	LONGS_EQUAL(0x7BDFF, EXTI->IMR);
	exti_disable_irq(EXTI_4);
	LONGS_EQUAL(0x7BDEF, EXTI->IMR);
}

TEST(EXTI, enable_event_ShouldSetEMR) {
	exti_enable_event(EXTI_0);
	LONGS_EQUAL(1, EXTI->EMR);
	exti_enable_event(EXTI_1);
	LONGS_EQUAL(3, EXTI->EMR);
	exti_enable_event(EXTI_5);
	LONGS_EQUAL(0x23, EXTI->EMR);
	exti_enable_event(EXTI_8);
	LONGS_EQUAL(0x123, EXTI->EMR);
	exti_enable_event(EXTI_19);
	LONGS_EQUAL(0x80123, EXTI->EMR);
	exti_enable_event(EXTI_14);
	LONGS_EQUAL(0x84123, EXTI->EMR);
}
TEST(EXTI, disable_event_ShouldClearEMR) {
	EXTI->EMR = 0xFFFFF;
	exti_disable_event(EXTI_19);
	LONGS_EQUAL(0x7FFFF, EXTI->EMR);
	exti_disable_event(EXTI_14);
	LONGS_EQUAL(0x7BFFF, EXTI->EMR);
	exti_disable_event(EXTI_9);
	LONGS_EQUAL(0x7BDFF, EXTI->EMR);
	exti_disable_event(EXTI_4);
	LONGS_EQUAL(0x7BDEF, EXTI->EMR);
}

TEST(EXTI, set_source_ShouldSetEXTICR_WhenGpioAGiven) {
	for (uint32_t i = 0; i < 16; i++) {
		exti_set_source(PERIPH_GPIOA, i);
	}

	LONGS_EQUAL(0, SYSCFG->CR1);
	LONGS_EQUAL(0, SYSCFG->CR2);
	LONGS_EQUAL(0, SYSCFG->CR3);
	LONGS_EQUAL(0, SYSCFG->CR4);
}
TEST(EXTI, set_source_ShouldSetEXTICR_WhenGpioBGiven) {
	for (uint32_t i = 0; i < 16; i++) {
		exti_set_source(PERIPH_GPIOB, i);
	}

	LONGS_EQUAL(0x1111, SYSCFG->CR1);
	LONGS_EQUAL(0x1111, SYSCFG->CR2);
	LONGS_EQUAL(0x1111, SYSCFG->CR3);
	LONGS_EQUAL(0x1111, SYSCFG->CR4);
}
TEST(EXTI, set_source_ShouldSetEXTICR_WhenGpioCGiven) {
	for (uint32_t i = 0; i < 16; i++) {
		exti_set_source(PERIPH_GPIOC, i);
	}

	LONGS_EQUAL(0x2222, SYSCFG->CR1);
	LONGS_EQUAL(0x2222, SYSCFG->CR2);
	LONGS_EQUAL(0x2222, SYSCFG->CR3);
	LONGS_EQUAL(0x2222, SYSCFG->CR4);
}
TEST(EXTI, set_source_ShouldSetEXTICR_WhenGpioDGiven) {
	for (uint32_t i = 0; i < 16; i++) {
		exti_set_source(PERIPH_GPIOD, i);
	}

	LONGS_EQUAL(0x3333, SYSCFG->CR1);
	LONGS_EQUAL(0x3333, SYSCFG->CR2);
	LONGS_EQUAL(0x3333, SYSCFG->CR3);
	LONGS_EQUAL(0x3333, SYSCFG->CR4);
}

TEST(EXTI, set_rising_trigger_ShouldSetRTSR) {
	exti_set_rising_trigger(EXTI_0);
	LONGS_EQUAL(1, EXTI->RTSR);
	exti_set_rising_trigger(EXTI_1);
	LONGS_EQUAL(3, EXTI->RTSR);
	exti_set_rising_trigger(EXTI_5);
	LONGS_EQUAL(0x23, EXTI->RTSR);
	exti_set_rising_trigger(EXTI_8);
	LONGS_EQUAL(0x123, EXTI->RTSR);
	exti_set_rising_trigger(EXTI_19);
	LONGS_EQUAL(0x80123, EXTI->RTSR);
	exti_set_rising_trigger(EXTI_14);
	LONGS_EQUAL(0x84123, EXTI->RTSR);
}
TEST(EXTI, clear_rising_trigger_ShouldClearRTSR) {
	EXTI->RTSR = 0xFFFFF;
	exti_clear_rising_trigger(EXTI_19);
	LONGS_EQUAL(0x7FFFF, EXTI->RTSR);
	exti_clear_rising_trigger(EXTI_14);
	LONGS_EQUAL(0x7BFFF, EXTI->RTSR);
	exti_clear_rising_trigger(EXTI_9);
	LONGS_EQUAL(0x7BDFF, EXTI->RTSR);
	exti_clear_rising_trigger(EXTI_4);
	LONGS_EQUAL(0x7BDEF, EXTI->RTSR);
}

TEST(EXTI, set_falling_trigger_ShouldSetFTSR) {
	exti_set_falling_trigger(EXTI_0);
	LONGS_EQUAL(1, EXTI->FTSR);
	exti_set_falling_trigger(EXTI_1);
	LONGS_EQUAL(3, EXTI->FTSR);
	exti_set_falling_trigger(EXTI_5);
	LONGS_EQUAL(0x23, EXTI->FTSR);
	exti_set_falling_trigger(EXTI_8);
	LONGS_EQUAL(0x123, EXTI->FTSR);
	exti_set_falling_trigger(EXTI_19);
	LONGS_EQUAL(0x80123, EXTI->FTSR);
	exti_set_falling_trigger(EXTI_14);
	LONGS_EQUAL(0x84123, EXTI->FTSR);
}
TEST(EXTI, clear_falling_trigger_ShouldClearFTSR) {
	EXTI->FTSR = 0xFFFFF;
	exti_clear_falling_trigger(EXTI_19);
	LONGS_EQUAL(0x7FFFF, EXTI->FTSR);
	exti_clear_falling_trigger(EXTI_14);
	LONGS_EQUAL(0x7BFFF, EXTI->FTSR);
	exti_clear_falling_trigger(EXTI_9);
	LONGS_EQUAL(0x7BDFF, EXTI->FTSR);
	exti_clear_falling_trigger(EXTI_4);
	LONGS_EQUAL(0x7BDEF, EXTI->FTSR);
}

TEST(EXTI, raise_irq_ShouldSetSWIER) {
	exti_raise_irq(EXTI_0);
	LONGS_EQUAL(1, EXTI->SWIER);
	exti_raise_irq(EXTI_1);
	LONGS_EQUAL(3, EXTI->SWIER);
	exti_raise_irq(EXTI_5);
	LONGS_EQUAL(0x23, EXTI->SWIER);
	exti_raise_irq(EXTI_8);
	LONGS_EQUAL(0x123, EXTI->SWIER);
	exti_raise_irq(EXTI_19);
	LONGS_EQUAL(0x80123, EXTI->SWIER);
	exti_raise_irq(EXTI_14);
	LONGS_EQUAL(0x84123, EXTI->SWIER);
}

TEST(EXTI, clear_event_ShouldClearPR) {
	EXTI->PR = 0xFFFFF;
	exti_clear_event(EXTI_19);
	LONGS_EQUAL(0x7FFFF, EXTI->PR);
	exti_clear_event(EXTI_14);
	LONGS_EQUAL(0x7BFFF, EXTI->PR);
	exti_clear_event(EXTI_9);
	LONGS_EQUAL(0x7BDFF, EXTI->PR);
	exti_clear_event(EXTI_4);
	LONGS_EQUAL(0x7BDEF, EXTI->PR);
}

TEST(EXTI, get_event_ShouldReturnPR) {
	EXTI->PR = 0xA5A5A5;
	LONGS_EQUAL(0xA5A5A5, exti_get_event());
}
