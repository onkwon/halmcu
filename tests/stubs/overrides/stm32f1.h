#ifndef STM32F1_OVERRIDE_H
#define STM32F1_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

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

extern DBGMCU_Type * const DBGMCU;

extern SYSCFG_Type * const SYSCFG;
extern EXTI_Type * const EXTI;
extern RCC_Type * const RCC;

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

#if defined(__cplusplus)
}
#endif

#endif
