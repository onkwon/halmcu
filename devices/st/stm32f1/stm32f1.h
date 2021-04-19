#ifndef STM32F1_H
#define STM32F1_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define PERIPHERAL_BASE			(0x40000000U)
#define APB1_BASE			(PERIPHERAL_BASE + 0)
#define APB2_BASE			(PERIPHERAL_BASE + 0x10000)
#define AHB_BASE			(PERIPHERAL_BASE + 0x18000)

#define SYSCFG_BASE			(APB2_BASE + 0)
#define EXTI_BASE			(APB2_BASE + 0x400)
#define RCC_BASE			(AHB_BASE + 0x9000)
#define GPIOA_BASE			(APB2_BASE + 0x800)
#define GPIOB_BASE			(APB2_BASE + 0xC00)
#define GPIOC_BASE			(APB2_BASE + 0x1000)
#define GPIOD_BASE			(APB2_BASE + 0x1400)
#define GPIOE_BASE			(APB2_BASE + 0x1800)
#define GPIOF_BASE			(APB2_BASE + 0x1C00)
#define GPIOG_BASE			(APB2_BASE + 0x2000)

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

#define SYSCFG				((SYSCFG_Type *)SYSCFG_BASE)
#define EXTI				((EXTI_Type *)EXTI_BASE)
#define RCC				((RCC_Type *)RCC_BASE)
#define GPIOA				((GPIO_Type *)GPIOA_BASE)
#define GPIOB				((GPIO_Type *)GPIOB_BASE)
#define GPIOC				((GPIO_Type *)GPIOC_BASE)
#define GPIOD				((GPIO_Type *)GPIOD_BASE)
#define GPIOE				((GPIO_Type *)GPIOE_BASE)
#define GPIOF				((GPIO_Type *)GPIOF_BASE)
#define GPIOG				((GPIO_Type *)GPIOG_BASE)

#if defined(__cplusplus)
}
#endif

#endif /* STM32F1_H */
