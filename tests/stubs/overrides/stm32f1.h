#ifndef STM32F1_OVERRIDE_H
#define STM32F1_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

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

extern SYSCFG_Type * const SYSCFG;
extern EXTI_Type * const EXTI;
extern RCC_Type * const RCC;

#if defined(__cplusplus)
}
#endif

#endif