#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include "abov/hal/clk.h"
#include "a33g.h"

#define MHZ				1000000U

extern "C" {
static struct pmu reg;
struct pmu * const PMU = &reg;
}

TEST_GROUP(Clock) {
	void setup(void) {
		clk_reset();
	}
	void teardown(void) {
	}
};

TEST(Clock, enable_peripheral_ShouldActivateClock) {
	clk_enable_peripheral(PERIPHERAL_ADC);
	LONGS_EQUAL(0x10000118, PMU->PCCR);
	clk_enable_peripheral(PERIPHERAL_UART2);
	LONGS_EQUAL(0x10400118, PMU->PCCR);
	clk_enable_peripheral(PERIPHERAL_I2C);
	LONGS_EQUAL(0x10440118, PMU->PCCR);
	clk_enable_peripheral(PERIPHERAL_SPI1);
	LONGS_EQUAL(0x10460118, PMU->PCCR);
	clk_enable_peripheral(PERIPHERAL_GPIO);
	LONGS_EQUAL(0x10460118, PMU->PCCR);
}

TEST(Clock, disable_peripheral_ShouldDeactivateClock) {
	PMU->PCCR = 0xffffffff;
	clk_disable_peripheral(PERIPHERAL_ADC);
	LONGS_EQUAL(0xefffffff, PMU->PCCR);
	clk_disable_peripheral(PERIPHERAL_GPIO);
	LONGS_EQUAL(0xeffffeff, PMU->PCCR);
	clk_disable_peripheral(PERIPHERAL_WDT);
	LONGS_EQUAL(0xeffffef7, PMU->PCCR);
	clk_disable_peripheral(PERIPHERAL_FRT);
	LONGS_EQUAL(0xeffffee7, PMU->PCCR);
}

TEST(Clock, enable_source_ShouldActivateClockSource) {
	clk_enable_source(CLK_EXTERNAL_OSC);
	LONGS_EQUAL(0x82, PMU->CCR);
	clk_enable_source(CLK_EXTERNAL_OSC_SUB);
	LONGS_EQUAL(0x8A, PMU->CCR);
	clk_enable_source(CLK_INTERNAL_OSC_16MHZ);
	LONGS_EQUAL(0xAA, PMU->CCR);
}

TEST(Clock, disable_source_ShouldDeactivateClockSource) {
	PMU->CCR = 0xAA;
	clk_disable_source(CLK_EXTERNAL_OSC);
	LONGS_EQUAL(0xA8, PMU->CCR);
	clk_disable_source(CLK_EXTERNAL_OSC_SUB);
	LONGS_EQUAL(0xA0, PMU->CCR);
	clk_disable_source(CLK_INTERNAL_OSC);
	LONGS_EQUAL(0x20, PMU->CCR);
	clk_disable_source(CLK_INTERNAL_OSC_16MHZ);
	LONGS_EQUAL(0x00, PMU->CCR);
}

TEST(Clock, set_source_ShouldSelectSourceClock) {
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(2, PMU->BCCR);
	clk_set_source(CLK_INTERNAL_OSC);
	LONGS_EQUAL(0, PMU->BCCR);
	clk_set_source(CLK_EXTERNAL_OSC_SUB);
	LONGS_EQUAL(1, PMU->BCCR);
}

TEST(Clock, get_source_ShouldReturnSourceClock) {
	LONGS_EQUAL(CLK_INTERNAL_OSC, clk_get_source());
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(CLK_PLL, clk_get_source());
	clk_set_source(CLK_EXTERNAL_OSC_SUB);
	LONGS_EQUAL(CLK_EXTERNAL_OSC_SUB, clk_get_source());
}

TEST(Clock, enable_pll_ShouldEnablePLL) {
	clk_enable_pll();
	LONGS_EQUAL(0xE000, PMU->PLLCON);
}

TEST(Clock, disable_pll_ShouldDisablePLL) {
	clk_enable_pll();
	clk_disable_pll();
	LONGS_EQUAL(0, PMU->PLLCON);
}

TEST(Clock, set_pll_freq_ShouldConfigurePLL_WhenOSC16MHzGiven) {
	LONGS_EQUAL(1, clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 8*MHZ));
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(0x300700, PMU->PLLCON & 0xFFFFFFF);
	LONGS_EQUAL(0x2, PMU->BCCR);
	LONGS_EQUAL(0xA0, PMU->CCR);

	LONGS_EQUAL(1, clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 75*MHZ));
	LONGS_EQUAL(0x4A10700, PMU->PLLCON & 0xFFFFFFF);
	LONGS_EQUAL(0x2, PMU->BCCR);
	LONGS_EQUAL(0xA0, PMU->CCR);
}

TEST(Clock, set_pll_freq_ShouldConfigurePLL_WhenXtalGiven) {
	LONGS_EQUAL(1, clk_set_pll_frequency(CLK_PLL, CLK_EXTERNAL_OSC, 8*MHZ));
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(0x300300, PMU->PLLCON & 0xFFFFFFF);
	LONGS_EQUAL(0x12, PMU->BCCR);
	LONGS_EQUAL(0x82, PMU->CCR);

	LONGS_EQUAL(1, clk_set_pll_frequency(CLK_PLL, CLK_EXTERNAL_OSC, 75*MHZ));
	LONGS_EQUAL(0x4A10300, PMU->PLLCON & 0xFFFFFFF);
	LONGS_EQUAL(0x12, PMU->BCCR);
	LONGS_EQUAL(0x82, PMU->CCR);
}

TEST(Clock, get_hclk_freq_ShouldReturnHclk) {
	LONGS_EQUAL(1*MHZ, clk_get_hclk_frequency());
	clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 8*MHZ);
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(8*MHZ, clk_get_hclk_frequency());
	clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 75*MHZ);
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(75*MHZ, clk_get_hclk_frequency());
	clk_set_pll_frequency(CLK_PLL, CLK_EXTERNAL_OSC, 75*MHZ);
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(75*MHZ, clk_get_hclk_frequency());
}

TEST(Clock, get_pclk_freq_ShouldReturnHclk) {
	LONGS_EQUAL(1*MHZ, clk_get_pclk_frequency());
	clk_set_pll_frequency(CLK_PLL, CLK_INTERNAL_OSC_16MHZ, 8*MHZ);
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(8*MHZ, clk_get_pclk_frequency());
	clk_set_pll_frequency(CLK_PLL, CLK_EXTERNAL_OSC, 32*MHZ);
	clk_set_source(CLK_PLL);
	LONGS_EQUAL(32*MHZ, clk_get_pclk_frequency());
}

TEST(Clock, is_pll_locked_ShouldReturnPllStatus) {
	LONGS_EQUAL(0, clk_is_pll_locked());
	PMU->PLLCON |= 1U << 12;
	LONGS_EQUAL(1, clk_is_pll_locked());
}
