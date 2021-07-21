#include "halmcu/ll/clk.h"

#include <stddef.h>

#include "halmcu/bitop.h"
#include "halmcu/compiler.h"
#include "halmcu/assert.h"
#include "stm32f1.h"

#define MHZ					1000000U
#define KHZ					1000U

#if !defined(F_HSI)
#define F_HSI					(8000000U)
#endif

static int get_ahb_activation_bitpos_from_peri(periph_t peri)
{
	switch (peri) {
	case PERIPH_SDIO:
		return 10;
	case PERIPH_FSMC:
		return 8;
	case PERIPH_FLASH: /* FLITF */
		return 4;
	case PERIPH_DMA2:
		return 1;
	case PERIPH_DMA1:
		return 0;
	default:
		return -1;
	}
}

static int get_apb2_activation_bitpos_from_peri(periph_t peri)
{
	switch (peri) {
	case PERIPH_ADC3:
		return 15;
	case PERIPH_USART1:
		return 14;
	case PERIPH_TIM8:
		return 13;
	case PERIPH_SPI1:
		return 12;
	case PERIPH_TIM1:
		return 11;
	case PERIPH_ADC2:
		return 10;
	case PERIPH_ADC1:
		return 9;
	case PERIPH_GPIOG:
		return 8;
	case PERIPH_GPIOF:
		return 7;
	case PERIPH_GPIOE:
		return 6;
	case PERIPH_GPIOD:
		return 5;
	case PERIPH_GPIOC:
		return 4;
	case PERIPH_GPIOB:
		return 3;
	case PERIPH_GPIOA:
		return 2;
	case PERIPH_AFIO:
		return 0;
	default:
		return -1;
	}
}

static int get_apb1_activation_bitpos_from_peri(periph_t peri)
{
	switch (peri) {
	case PERIPH_PWR:
		return 28;
	case PERIPH_BKP:
		return 27;
	case PERIPH_USB:
		return 23;
	case PERIPH_I2C2:
		return 22;
	case PERIPH_I2C1:
		return 21;
	case PERIPH_UART5:
		return 20;
	case PERIPH_UART4:
		return 19;
	case PERIPH_USART3:
		return 18;
	case PERIPH_USART2:
		return 17;
	case PERIPH_SPI3:
		return 15;
	case PERIPH_SPI2:
		return 14;
	case PERIPH_WDT:
		return 11;
	case PERIPH_TIM7:
		return 5;
	case PERIPH_TIM6:
		return 4;
	case PERIPH_TIM5:
		return 3;
	case PERIPH_TIM4:
		return 2;
	case PERIPH_TIM3:
		return 1;
	case PERIPH_TIM2:
		return 0;
	default:
		return -1;
	}
}

static int get_activation_bitpos_and_reg(periph_t peri, volatile uint32_t **reg)
{
	int bitpos = get_ahb_activation_bitpos_from_peri(peri);

	if (bitpos >= 0) {
		*reg = &RCC->AHBENR;
	} else if ((bitpos = get_apb2_activation_bitpos_from_peri(peri)) >= 0) {
		*reg = &RCC->APB2ENR;
	} else if ((bitpos = get_apb1_activation_bitpos_from_peri(peri)) >= 0) {
		*reg = &RCC->APB1ENR;
	}

	return bitpos;
}

static clk_source_t get_sysclk_source(void)
{
	switch ((RCC->CFGR >> 2) & 0x3) { /* SWS */
	case 1:
		return CLK_HSE;
	case 2:
		return CLK_PLL;
	case 0: /* fall through */
	default:
		return CLK_HSI;
	}
}

static uint32_t get_pllclk(void)
{
	uint32_t reg = RCC->CFGR;
	uint32_t pllm = ((reg >> 18) & 0xf) + 2; /* PLLMUL */

	if (((reg >> 16) & 0x1) == 0) {
		return (F_HSI >> 1) * pllm;
	}

	if (((reg >> 17) & 1) == 0) { /* PLLXTPRE */
		return F_HSE * pllm;
	}

	return (F_HSE >> 1) * pllm;
}

static uint32_t get_hclk(void)
{
	switch (get_sysclk_source()) {
	case CLK_HSI:
		return F_HSI;
	case CLK_HSE:
		return F_HSE;
	case CLK_PLL:
		break;
	default:
		return 0;
	}

	uint32_t pllclk = get_pllclk();
	uint32_t pre = (RCC->CFGR >> 4) & 0xf; /* mask HPRE[7:4] */
	uint32_t shift_factor = (pre == 0)? 0 : pre - 7;

	return pllclk >> shift_factor;
}

static uint32_t get_frequency(clk_source_t clk)
{
	switch (clk) {
	case CLK_PLL:
		return get_hclk();
	case CLK_LSI:
		return 40*KHZ;
	case CLK_HSI:
		return F_HSI;
	case CLK_HSE:
		return F_HSE;
	case CLK_LSE:
		return 32768U;
	default:
		return 0;
	}
}

static uint32_t get_pclk2_frequency(void)
{
	uint32_t hclk = get_hclk();
	uint32_t pre = (RCC->CFGR >> 11) & 0x7; /* PPRE2 */
	uint32_t shift_factor = (pre == 0)? 0 : pre - 3;
	return hclk >> shift_factor;
}

static uint32_t get_pclk1_frequency(void)
{
	uint32_t hclk = get_hclk();
	uint32_t pre = (RCC->CFGR >> 8) & 0x7; /* PPRE1 */
	uint32_t shift_factor = (pre == 0)? 0 : pre - 3;
	return hclk >> shift_factor;
}

static void set_pclk1_frequency(uint32_t hz)
{
	if (hz <= 36*MHZ) {
		bitop_clean_set_with_mask(&RCC->CFGR, 8, 7, 0);
		return;
	}

	for (uint32_t i = 0; i < 4; i++) { // 2, 4, 8, 16
		if ((hz >> (i+1)) <= 36*MHZ) {
			bitop_clean_set_with_mask(&RCC->CFGR, 8, 7, i | 4);
			break;
		}
	}
}

void clk_ll_enable_peripheral(periph_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_set(reg, (uint32_t)bitpos);
	}
}

void clk_ll_disable_peripheral(periph_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_clear(reg, (uint32_t)bitpos);
	}
}

uint32_t clk_ll_get_hclk_frequency(void)
{
	return get_hclk();
}

uint32_t clk_ll_get_pclk_frequency(void)
{
	return get_pclk1_frequency();
}

uint32_t clk_ll_get_frequency(clk_source_t clk)
{
	return get_frequency(clk);
}

void clk_ll_enable_source(clk_source_t clk)
{
	volatile uint32_t *reg = &RCC->CR;
	uint32_t bit = 0; /* HSI for the default */

	if (clk == CLK_LSI) {
		reg = &RCC->CSR;
	} else if (clk == CLK_LSE) {
		reg = &RCC->BDCR;
	} else if (clk == CLK_HSE) {
		bit = 16;
	}

	bitop_set(reg, bit);
	while (!(*reg & (1U << (bit + 1)))) {
		/* wait until ready */
	}
}

void clk_ll_disable_source(clk_source_t clk)
{
	volatile uint32_t *reg = &RCC->CR;
	uint32_t bit = 0;

	if (clk == CLK_LSI) {
		reg = &RCC->CSR;
	} else if (clk == CLK_LSE) {
		reg = &RCC->BDCR;
	} else if (clk == CLK_HSE) {
		bit = 16;
	}

	bitop_clear(reg, bit);
}

void clk_ll_start_pll(void)
{
	bitop_set(&RCC->CR, 24); /* PLLON */
}

void clk_ll_stop_pll(void)
{
	bitop_clear(&RCC->CR, 24); /* PLLON */
}

bool clk_ll_is_pll_locked(void)
{
	return !!(RCC->CR & (1U << 25)); /* PLLRDY */
}

void clk_ll_set_source(clk_source_t clk)
{
	uint8_t val = 0;

	if (clk == CLK_HSI) {
		val = 0;
	} else if (clk == CLK_HSE) {
		val = 1;
	} else if (clk == CLK_PLL) {
		val = 2;
	} else {
		assert(0);
	}

	bitop_clean_set_with_mask(&RCC->CFGR, 0, 3, val); /* SW */
	while (((RCC->CFGR >> 2) & 0x3) != val) {
		/* wait until ready */
	}
}

static bool get_pllmul_and_hpre(uint32_t *pllmul, uint32_t *hpre,
		uint32_t target_hz, uint32_t source_hz)
{
	uint32_t div[] = { 1, 2, 4, 8, 16, 64, 128, 256, 512 };

	for (uint32_t mul = 16; mul >= 2; mul--) {
		uint32_t hz = source_hz * mul;
		for (int i = 0; i < 9; i++) {
			if (hz / div[i] != target_hz) {
				continue;
			}

			*pllmul = mul - 2;
			*hpre = (i != 0)? (uint32_t)(8 | (i - 1)) : (uint32_t)i;

			return true;
		}
	}

	return false;
}

/* APB1 <= APB2 <= AHB
 * APB1 <= 36MHz
 * APB2 <= 72MHz
 * AHB <= 72MHz
 * ADC <= 14MHz
 * USB = 48MHz */
bool clk_ll_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz)
{
	if (clk != CLK_PLL) {
		return false;
	}
	if (clkin != CLK_HSI && clkin != CLK_HSE) {
		return false;
	}
	if (hz < 1*MHZ) {
		return false;
	}

	uint32_t src_hz = get_frequency(clkin);

	if (clkin == CLK_HSE) {
		bitop_set(&RCC->CFGR, 16); /* PLLSRC */
	} else { /* CLK_HSI */
		bitop_clear(&RCC->CFGR, 16); /* PLLSRC */
		src_hz >>= 1;
	}

	uint32_t pllmul, hpre;

	if (!get_pllmul_and_hpre(&pllmul, &hpre, hz, src_hz)) {
		return false;
	}

	bitop_clean_set_with_mask(&RCC->CFGR, 18, 0xf, pllmul);
	bitop_clean_set_with_mask(&RCC->CFGR, 4, 0xf, hpre);

	set_pclk1_frequency(hz);

	return true;
}

uint32_t clk_ll_get_peripheral_clock_source_frequency(periph_t peri)
{
	switch (peri) {
	case PERIPH_TIM1: /* fall through */
	case PERIPH_TIM8: /* fall through */
	case PERIPH_ADC1: /* fall through */
	case PERIPH_ADC2: /* fall through */
	case PERIPH_ADC3: /* fall through */
	case PERIPH_SPI1: /* fall through */
	case PERIPH_USART1:
		return get_pclk2_frequency();
	case PERIPH_TIM2: /* fall through */
	case PERIPH_TIM3: /* fall through */
	case PERIPH_TIM4: /* fall through */
	case PERIPH_TIM5: /* fall through */
	case PERIPH_TIM6: /* fall through */
	case PERIPH_TIM7: /* fall through */
	case PERIPH_SPI2: /* fall through */
	case PERIPH_SPI3: /* fall through */
	case PERIPH_USART2: /* fall through */
	case PERIPH_USART3: /* fall through */
	case PERIPH_UART4: /* fall through */
	case PERIPH_UART5:
		return get_pclk1_frequency();
	default:
		assert(0);
		return get_pclk1_frequency();
	}
}
