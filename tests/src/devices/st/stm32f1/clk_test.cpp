#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/clk.h"
#include "stm32f1.h"

extern "C" {
static RCC_Type rcc_reg;
RCC_Type * const RCC = &rcc_reg;
}

TEST_GROUP(CLK) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(RCC, 0, sizeof(*RCC));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(CLK, enable_peripheral_ShouldSetAHBENR_WhenAHBPeriphGiven) {
	clk_enable_peripheral(PERIPH_SDIO);
	LONGS_EQUAL(0x400, RCC->AHBENR);
	clk_enable_peripheral(PERIPH_FSMC);
	LONGS_EQUAL(0x500, RCC->AHBENR);
	clk_enable_peripheral(PERIPH_DMA2);
	LONGS_EQUAL(0x502, RCC->AHBENR);
	clk_enable_peripheral(PERIPH_DMA1);
	LONGS_EQUAL(0x503, RCC->AHBENR);
}
TEST(CLK, disable_peripheral_ShouldClearAHBENR_WhenAHBPeriphGiven) {
	RCC->AHBENR = 0xFFFF;
	clk_disable_peripheral(PERIPH_SDIO);
	LONGS_EQUAL(0xFBFF, RCC->AHBENR);
	clk_disable_peripheral(PERIPH_FSMC);
	LONGS_EQUAL(0xFAFF, RCC->AHBENR);
	clk_disable_peripheral(PERIPH_DMA2);
	LONGS_EQUAL(0xFAFD, RCC->AHBENR);
	clk_disable_peripheral(PERIPH_DMA1);
	LONGS_EQUAL(0xFAFC, RCC->AHBENR);
}

TEST(CLK, enable_peripheral_ShouldSetAPB2ENR_WhenAPB2PeriphGiven) {
	clk_enable_peripheral(PERIPH_ADC3);
	LONGS_EQUAL(0x8000, RCC->APB2ENR);
	clk_enable_peripheral(PERIPH_ADC2);
	LONGS_EQUAL(0x8400, RCC->APB2ENR);
	clk_enable_peripheral(PERIPH_GPIOD);
	LONGS_EQUAL(0x8420, RCC->APB2ENR);
	clk_enable_peripheral(PERIPH_AFIO);
	LONGS_EQUAL(0x8421, RCC->APB2ENR);
}
TEST(CLK, disable_peripheral_ShouldSetAPB2ENR_WhenAPB2PeriphGiven) {
	RCC->APB2ENR = 0xFFFF;
	clk_disable_peripheral(PERIPH_SPI1);
	LONGS_EQUAL(0xEFFF, RCC->APB2ENR);
	clk_disable_peripheral(PERIPH_ADC1);
	LONGS_EQUAL(0xEDFF, RCC->APB2ENR);
	clk_disable_peripheral(PERIPH_GPIOE);
	LONGS_EQUAL(0xEDBF, RCC->APB2ENR);
	clk_disable_peripheral(PERIPH_GPIOB);
	LONGS_EQUAL(0xEDB7, RCC->APB2ENR);
}

TEST(CLK, enable_peripheral_ShouldSetAPB1ENR_WhenAPB1PeriphGiven) {
	clk_enable_peripheral(PERIPH_PWR);
	LONGS_EQUAL(0x10000000, RCC->APB1ENR);
	clk_enable_peripheral(PERIPH_I2C1);
	LONGS_EQUAL(0x10200000, RCC->APB1ENR);
	clk_enable_peripheral(PERIPH_UART4);
	LONGS_EQUAL(0x10280000, RCC->APB1ENR);
	clk_enable_peripheral(PERIPH_WDT);
	LONGS_EQUAL(0x10280800, RCC->APB1ENR);
	clk_enable_peripheral(PERIPH_TIM4);
	LONGS_EQUAL(0x10280804, RCC->APB1ENR);
}
TEST(CLK, disable_peripheral_ShouldSetAPB1ENR_WhenAPB1PeriphGiven) {
	RCC->APB1ENR = 0xFFFFFFFF;
	clk_disable_peripheral(PERIPH_PWR);
	LONGS_EQUAL(0xEFFFFFFF, RCC->APB1ENR);
	clk_disable_peripheral(PERIPH_I2C1);
	LONGS_EQUAL(0xEFDFFFFF, RCC->APB1ENR);
	clk_disable_peripheral(PERIPH_UART4);
	LONGS_EQUAL(0xEFD7FFFF, RCC->APB1ENR);
	clk_disable_peripheral(PERIPH_WDT);
	LONGS_EQUAL(0xEFD7F7FF, RCC->APB1ENR);
	clk_disable_peripheral(PERIPH_TIM4);
	LONGS_EQUAL(0xEFD7F7FB, RCC->APB1ENR);
}

TEST(CLK, get_hclk_freq_ShouldReturnHSI_WhenResetStatusGiven) {
	LONGS_EQUAL(8000000, clk_get_hclk_frequency());
}
