#include "abov/ll/clk.h"
#include <assert.h>
#include <stddef.h>
#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/asm/arm/cmsis.h"
#include "stm32f1.h"

#define MHZ				1000000
#if !defined(F_HSI)
#define F_HSI				(8*MHZ)
#endif

static int get_ahb_activation_bitpos_from_peri(peripheral_t peri)
{
	switch (peri) {
	case PERI_SDIO:
		return 10;
	case PERI_FSMC:
		return 8;
	case PERI_FLASH: /* FLITF */
		return 4;
	case PERI_DMA2:
		return 1;
	case PERI_DMA1:
		return 0;
	default:
		return -1;
	}
}

static int get_apb2_activation_bitpos_from_peri(peripheral_t peri)
{
	switch (peri) {
	case PERI_ADC3:
		return 15;
	case PERI_USART1:
		return 14;
	case PERI_TIM8:
		return 13;
	case PERI_SPI1:
		return 12;
	case PERI_TIM1:
		return 11;
	case PERI_ADC2:
		return 10;
	case PERI_ADC1:
		return 9;
	case PERI_GPIOG:
		return 8;
	case PERI_GPIOF:
		return 7;
	case PERI_GPIOE:
		return 6;
	case PERI_GPIOD:
		return 5;
	case PERI_GPIOC:
		return 4;
	case PERI_GPIOB:
		return 3;
	case PERI_GPIOA:
		return 2;
	case PERI_AFIO:
		return 0;
	default:
		return -1;
	}
}

static int get_apb1_activation_bitpos_from_peri(peripheral_t peri)
{
	switch (peri) {
	case PERI_PWR:
		return 28;
	case PERI_BKP:
		return 27;
	case PERI_USB:
		return 23;
	case PERI_I2C2:
		return 22;
	case PERI_I2C1:
		return 21;
	case PERI_UART5:
		return 20;
	case PERI_UART4:
		return 19;
	case PERI_USART3:
		return 18;
	case PERI_USART2:
		return 17;
	case PERI_SPI3:
		return 15;
	case PERI_SPI2:
		return 14;
	case PERI_WDT:
		return 11;
	case PERI_TIM7:
		return 5;
	case PERI_TIM6:
		return 4;
	case PERI_TIM5:
		return 3;
	case PERI_TIM4:
		return 2;
	case PERI_TIM3:
		return 1;
	case PERI_TIM2:
		return 0;
	default:
		return -1;
	}
}

static int get_activation_bitpos_and_reg(peripheral_t peri,
		volatile uint32_t **reg)
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

void clk_enable_peripheral(peripheral_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_set(reg, (uint32_t)bitpos);
	}
}

void clk_disable_peripheral(peripheral_t peri)
{
	volatile uint32_t *reg = NULL;
	int bitpos = get_activation_bitpos_and_reg(peri, &reg);

	if (reg != NULL) {
		bitop_clear(reg, (uint32_t)bitpos);
	}
}

uint32_t clk_get_hclk_frequency(void)
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
