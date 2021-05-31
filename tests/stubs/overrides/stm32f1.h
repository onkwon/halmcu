#ifndef STM32F1_OVERRIDE_H
#define STM32F1_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define PERIPHERAL_BASE			(0x40000000U)
#define APB1_BASE			(PERIPHERAL_BASE + 0)
#define APB2_BASE			(PERIPHERAL_BASE + 0x10000U)
#define AHB_BASE			(PERIPHERAL_BASE + 0x20000U)

typedef struct {
	volatile uint32_t IDCODE;
	volatile uint32_t CR;
} DBGMCU_Type;

typedef struct {
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t CR4;
	volatile uint32_t MAPR2;
} SYSCFG_Type;

typedef struct {
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
} EXTI_Type;

typedef struct {
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
} RCC_Type;

typedef struct {
	volatile uint32_t ACR;
	volatile uint32_t KEYR;
	volatile uint32_t OPTKEYR;
	volatile uint32_t SR;
	volatile uint32_t CR;
	volatile uint32_t AR;
	volatile uint32_t reserved;
	volatile uint32_t OBR;
	volatile uint32_t WRPR;
} FLASH_Type;

typedef struct {
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;
} GPIO_Type;

typedef struct {
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t BRR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t CR3;
	volatile uint32_t GTPR;
} USART_Type;

typedef struct {
	volatile uint32_t KR;
	volatile uint32_t PR;
	volatile uint32_t RLP;
	volatile uint32_t SR;
} IWDT_Type;

typedef struct {
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR1;
	volatile uint32_t SMPR2;
	volatile uint32_t JOFR1;
	volatile uint32_t JOFR2;
	volatile uint32_t JOFR3;
	volatile uint32_t JOFR4;
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR1;
	volatile uint32_t SQR2;
	volatile uint32_t SQR3;
	volatile uint32_t JSQR;
	volatile uint32_t JDR1;
	volatile uint32_t JDR2;
	volatile uint32_t JDR3;
	volatile uint32_t JDR4;
	volatile uint32_t DR;
} ADC_Type;

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMCR;
	volatile uint32_t DIER;
	volatile uint32_t SR;
	volatile uint32_t EGR;
	volatile uint32_t CCMR1;
	volatile uint32_t CCMR2;
	volatile uint32_t CCER;
	volatile uint32_t CNT;
	volatile uint32_t PSC;
	volatile uint32_t ARR;
	volatile uint32_t RCR;
	volatile uint32_t CCR1;
	volatile uint32_t CCR2;
	volatile uint32_t CCR3;
	volatile uint32_t CCR4;
	volatile uint32_t BDTR;
	volatile uint32_t DCR;
	volatile uint32_t DMAR;
} TIM_Type;

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
} SPI_Type;

extern DBGMCU_Type * const DBGMCU;

extern SYSCFG_Type * const SYSCFG;
extern EXTI_Type * const EXTI;
extern RCC_Type * const RCC;
extern FLASH_Type * const FLASH;

extern GPIO_Type * const GPIOA;
extern GPIO_Type * const GPIOB;
extern GPIO_Type * const GPIOC;
extern GPIO_Type * const GPIOD;
extern GPIO_Type * const GPIOE;
extern GPIO_Type * const GPIOF;
extern GPIO_Type * const GPIOG;

extern USART_Type * const USART1;
extern USART_Type * const USART2;
extern USART_Type * const USART3;
extern USART_Type * const UART4;
extern USART_Type * const UART5;

extern IWDT_Type * const IWDT;

extern ADC_Type * const ADC1;
extern ADC_Type * const ADC2;
extern ADC_Type * const ADC3;

extern TIM_Type * const TIM1;
extern TIM_Type * const TIM2;
extern TIM_Type * const TIM3;
extern TIM_Type * const TIM4;
extern TIM_Type * const TIM5;
extern TIM_Type * const TIM6;
extern TIM_Type * const TIM7;
extern TIM_Type * const TIM8;
extern TIM_Type * const TIM9;
extern TIM_Type * const TIM10;
extern TIM_Type * const TIM11;
extern TIM_Type * const TIM12;
extern TIM_Type * const TIM13;
extern TIM_Type * const TIM14;

extern SPI_Type * const SPI1;
extern SPI_Type * const SPI2;
extern SPI_Type * const SPI3;

#if defined(__cplusplus)
}
#endif

#endif
