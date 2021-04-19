#include "abov/ll/clk.h"
#include <assert.h>
#include <stddef.h>
#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/asm/arm/cmsis.h"
#include "stm32f1.h"

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
