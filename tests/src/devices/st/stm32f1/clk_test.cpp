#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "halmcu/ll/clk.h"
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
	clk_ll_enable_peripheral(PERIPH_SDIO);
	LONGS_EQUAL(0x400, RCC->AHBENR);
	clk_ll_enable_peripheral(PERIPH_FSMC);
	LONGS_EQUAL(0x500, RCC->AHBENR);
	clk_ll_enable_peripheral(PERIPH_DMA2);
	LONGS_EQUAL(0x502, RCC->AHBENR);
	clk_ll_enable_peripheral(PERIPH_DMA1);
	LONGS_EQUAL(0x503, RCC->AHBENR);
}
TEST(CLK, disable_peripheral_ShouldClearAHBENR_WhenAHBPeriphGiven) {
	RCC->AHBENR = 0xFFFF;
	clk_ll_disable_peripheral(PERIPH_SDIO);
	LONGS_EQUAL(0xFBFF, RCC->AHBENR);
	clk_ll_disable_peripheral(PERIPH_FSMC);
	LONGS_EQUAL(0xFAFF, RCC->AHBENR);
	clk_ll_disable_peripheral(PERIPH_DMA2);
	LONGS_EQUAL(0xFAFD, RCC->AHBENR);
	clk_ll_disable_peripheral(PERIPH_DMA1);
	LONGS_EQUAL(0xFAFC, RCC->AHBENR);
}

TEST(CLK, enable_peripheral_ShouldSetAPB2ENR_WhenAPB2PeriphGiven) {
	clk_ll_enable_peripheral(PERIPH_ADC3);
	LONGS_EQUAL(0x8000, RCC->APB2ENR);
	clk_ll_enable_peripheral(PERIPH_ADC2);
	LONGS_EQUAL(0x8400, RCC->APB2ENR);
	clk_ll_enable_peripheral(PERIPH_GPIOD);
	LONGS_EQUAL(0x8420, RCC->APB2ENR);
	clk_ll_enable_peripheral(PERIPH_AFIO);
	LONGS_EQUAL(0x8421, RCC->APB2ENR);
}
TEST(CLK, disable_peripheral_ShouldSetAPB2ENR_WhenAPB2PeriphGiven) {
	RCC->APB2ENR = 0xFFFF;
	clk_ll_disable_peripheral(PERIPH_SPI1);
	LONGS_EQUAL(0xEFFF, RCC->APB2ENR);
	clk_ll_disable_peripheral(PERIPH_ADC1);
	LONGS_EQUAL(0xEDFF, RCC->APB2ENR);
	clk_ll_disable_peripheral(PERIPH_GPIOE);
	LONGS_EQUAL(0xEDBF, RCC->APB2ENR);
	clk_ll_disable_peripheral(PERIPH_GPIOB);
	LONGS_EQUAL(0xEDB7, RCC->APB2ENR);
}

TEST(CLK, enable_peripheral_ShouldSetAPB1ENR_WhenAPB1PeriphGiven) {
	clk_ll_enable_peripheral(PERIPH_PWR);
	LONGS_EQUAL(0x10000000, RCC->APB1ENR);
	clk_ll_enable_peripheral(PERIPH_I2C1);
	LONGS_EQUAL(0x10200000, RCC->APB1ENR);
	clk_ll_enable_peripheral(PERIPH_UART4);
	LONGS_EQUAL(0x10280000, RCC->APB1ENR);
	clk_ll_enable_peripheral(PERIPH_WDT);
	LONGS_EQUAL(0x10280800, RCC->APB1ENR);
	clk_ll_enable_peripheral(PERIPH_TIM4);
	LONGS_EQUAL(0x10280804, RCC->APB1ENR);
}
TEST(CLK, disable_peripheral_ShouldSetAPB1ENR_WhenAPB1PeriphGiven) {
	RCC->APB1ENR = 0xFFFFFFFF;
	clk_ll_disable_peripheral(PERIPH_PWR);
	LONGS_EQUAL(0xEFFFFFFF, RCC->APB1ENR);
	clk_ll_disable_peripheral(PERIPH_I2C1);
	LONGS_EQUAL(0xEFDFFFFF, RCC->APB1ENR);
	clk_ll_disable_peripheral(PERIPH_UART4);
	LONGS_EQUAL(0xEFD7FFFF, RCC->APB1ENR);
	clk_ll_disable_peripheral(PERIPH_WDT);
	LONGS_EQUAL(0xEFD7F7FF, RCC->APB1ENR);
	clk_ll_disable_peripheral(PERIPH_TIM4);
	LONGS_EQUAL(0xEFD7F7FB, RCC->APB1ENR);
}

TEST(CLK, get_hclk_freq_ShouldReturnHSI_WhenResetStatusGiven) {
	LONGS_EQUAL(8000000, clk_ll_get_hclk_frequency());
}
TEST(CLK, get_hclk_freq_ShouldReturnHSE_WhenSWSIsOne) {
	RCC->CFGR |= 4;
	LONGS_EQUAL(8000000, clk_ll_get_hclk_frequency());
}
TEST(CLK, get_hclk_freq_ShouldReturnPLL_WhenSWSIsTwo) {
	RCC->CFGR |= 8;
	LONGS_EQUAL(8000000, clk_ll_get_hclk_frequency());
}
TEST(CLK, get_hclk_freq_ShouldReturnPLL_WhenSWSIsTwoAndHSEGiven) {
	RCC->CFGR |= 8 | 0x10000;
	LONGS_EQUAL(8000000*2, clk_ll_get_hclk_frequency());
}
TEST(CLK, get_hclk_freq_ShouldReturnPLL_WhenSWSIsTwoAndHSEAndXtpreGiven) {
	RCC->CFGR |= 8 | 0x30000;
	LONGS_EQUAL(8000000, clk_ll_get_hclk_frequency());
}

TEST(CLK, get_pclk_freq_ShouldReturnHSI_WhenResetStatusGiven) {
	LONGS_EQUAL(8000000, clk_ll_get_pclk_frequency());
}

TEST(CLK, get_frequency_ShouldReturnClockFrequency) {
	LONGS_EQUAL(40000, clk_ll_get_frequency(CLK_LSI));
	LONGS_EQUAL(8000000, clk_ll_get_frequency(CLK_HSI));
	LONGS_EQUAL(32768, clk_ll_get_frequency(CLK_LSE));
	LONGS_EQUAL(8000000, clk_ll_get_frequency(CLK_HSE));
	LONGS_EQUAL(8000000, clk_ll_get_frequency(CLK_PLL));
}

TEST(CLK, enable_source_ShouldEnableLSI_WhenLSIGiven) {
	RCC->CSR = 2;
	clk_ll_enable_source(CLK_LSI);
	LONGS_EQUAL(3, RCC->CSR);
}
TEST(CLK, enable_source_ShouldEnableLSE_WhenLSEGiven) {
	RCC->BDCR = 2;
	clk_ll_enable_source(CLK_LSE);
	LONGS_EQUAL(3, RCC->BDCR);
}
TEST(CLK, enable_source_ShouldEnableHSI_WhenHSIGiven) {
	RCC->CR = 2;
	clk_ll_enable_source(CLK_HSI);
	LONGS_EQUAL(3, RCC->CR);
}
TEST(CLK, enable_source_ShouldEnableHSE_WhenHSEGiven) {
	RCC->CR = 0x20000;
	clk_ll_enable_source(CLK_HSE);
	LONGS_EQUAL(0x30000, RCC->CR);
}

TEST(CLK, disable_source_ShouldDisableLSI_WhenLSIGiven) {
	RCC->CSR = 1;
	clk_ll_disable_source(CLK_LSI);
	LONGS_EQUAL(0, RCC->CSR);
}
TEST(CLK, disable_source_ShouldDisableHSI_WhenLSIGiven) {
	RCC->CR = 1;
	clk_ll_disable_source(CLK_HSI);
	LONGS_EQUAL(0, RCC->CR);
}
TEST(CLK, disable_source_ShouldDisableLSE_WhenLSEGiven) {
	RCC->BDCR = 1;
	clk_ll_disable_source(CLK_LSE);
	LONGS_EQUAL(0, RCC->BDCR);
}
TEST(CLK, disable_source_ShouldDisableHSE_WhenHSEGiven) {
	RCC->CR = 0x10000;
	clk_ll_disable_source(CLK_HSE);
	LONGS_EQUAL(0, RCC->CR);
}

TEST(CLK, start_pll_ShouldSetPllOn) {
	clk_ll_start_pll();
	LONGS_EQUAL(0x1000000, RCC->CR);
}

TEST(CLK, stop_pll_ShouldClearPllOn) {
	RCC->CR = 0x1000000;
	clk_ll_stop_pll();
	LONGS_EQUAL(0, RCC->CR);
}

TEST(CLK, is_pll_locked_ShouldReturnItsState) {
	LONGS_EQUAL(false, clk_ll_is_pll_locked());
	RCC->CR = 0x2000000;
	LONGS_EQUAL(true, clk_ll_is_pll_locked());
}

TEST(CLK, set_source_ShouldSetSystemClockSource_WhenHSEGiven) {
	RCC->CFGR = 4;
	clk_ll_set_source(CLK_HSE);
	LONGS_EQUAL(5, RCC->CFGR);
}
TEST(CLK, set_source_ShouldSetSystemClockSource_WhenHSIGiven) {
	clk_ll_set_source(CLK_HSI);
	LONGS_EQUAL(0, RCC->CFGR);
}
TEST(CLK, set_source_ShouldSetSystemClockSource_WhenPLLGiven) {
	RCC->CFGR = 8;
	clk_ll_set_source(CLK_PLL);
	LONGS_EQUAL(0xA, RCC->CFGR);
}

TEST(CLK, set_pll_frequency_ShouldReturnFalse_WhenTargetClkIsNotPLL) {
	// It supports pll only.
	LONGS_EQUAL(false, clk_ll_set_pll_frequency(CLK_HSI, CLK_HSI, 100000));
}
TEST(CLK, set_pll_frequency_ShouldReturnFalse_WhenInvalidClockSourceGiven) {
	LONGS_EQUAL(false, clk_ll_set_pll_frequency(CLK_PLL, CLK_LSI, 1000000));
}
TEST(CLK, set_pll_frequency_ShouldReturnFalse_WhenTargetFreqIsTooLow) {
	LONGS_EQUAL(false, clk_ll_set_pll_frequency(CLK_PLL, CLK_HSI, 100000));
}
TEST(CLK, set_pll_frequency_ShouldReturnTrue_When72MHzWithHSIAsSourceGiven) {
	RCC->CFGR = 0xA;
	LONGS_EQUAL(true, clk_ll_set_pll_frequency(CLK_PLL, CLK_HSI, 64000000));
	clk_ll_set_source(CLK_PLL);
	LONGS_EQUAL(64000000, clk_ll_get_hclk_frequency());
}
TEST(CLK, set_pll_frequency_ShouldReturnTrue_When72MHzWithHSEAsSourceGiven) {
	RCC->CFGR = 0xA;
	LONGS_EQUAL(true, clk_ll_set_pll_frequency(CLK_PLL, CLK_HSE, 72000000));
	clk_ll_set_source(CLK_PLL);
	LONGS_EQUAL(72000000, clk_ll_get_hclk_frequency());
}
TEST(CLK, set_pll_frequency_ShouldSetPclk1EqualOrLessThen36MHz) {
	RCC->CFGR = 0xA;
	clk_ll_set_pll_frequency(CLK_PLL, CLK_HSE, 72000000);
	clk_ll_set_source(CLK_PLL);
	LONGS_EQUAL(36000000, clk_ll_get_pclk_frequency());
	clk_ll_set_pll_frequency(CLK_PLL, CLK_HSE, 64000000);
	LONGS_EQUAL(32000000, clk_ll_get_pclk_frequency());
}
