#include "abov/ll/clk.h"

#include <stddef.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
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

void clk_enable_peripheral(periph_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_set(reg, (uint32_t)bitpos);
	}
}

void clk_disable_peripheral(periph_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_clear(reg, (uint32_t)bitpos);
	}
}

uint32_t clk_get_hclk_frequency(void)
{
	return get_hclk();
}

uint32_t clk_get_pclk_frequency(void)
{
	uint32_t hclk = get_hclk();
	uint32_t pre = (RCC->CFGR >> 8) & 0x7; /* PPRE1 */
	uint32_t shift_factor = (pre == 0)? 0 : pre - 3;
	return hclk >> shift_factor;
}

uint32_t clk_get_frequency(clk_source_t clk)
{
	switch (clk) {
	case CLK_PLL:
		return get_hclk();
	case CLK_LSI:
		return 40*KHZ;
	case CLK_HSI:
		return 8*MHZ;
	case CLK_HSE:
		return F_HSE;
	case CLK_LSE:
		return 32768U;
	default:
		return 0;
	}
}

#if 0
uint32_t clk_get_pclk2_frequency(void)
{
	uint32_t hclk = get_hclk();
	uint32_t pre = (RCC->CFGR >> 11) & 0x7; /* PPRE2 */
	uint32_t shift_factor = (pre == 0)? 0 : pre - 3;
	return hclk >> shift_factor;
}
#endif
