#ifndef STM32F1_H
#define STM32F1_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define PERIPHERAL_BASE			(0x40000000U)
#define APB1_BASE			(PERIPHERAL_BASE + 0)
#define APB2_BASE			(PERIPHERAL_BASE + 0x10000U)
#define AHB_BASE			(PERIPHERAL_BASE + 0x20000U)

#define DBGMCU_BASE			(0xE0042000U)

#define RCC_BASE			(AHB_BASE + 0x1000U)
#define FLASH_BASE			(AHB_BASE + 0x2000U)

#define SYSCFG_BASE			(APB2_BASE + 0)
#define EXTI_BASE			(APB2_BASE + 0x400U)
#define GPIOA_BASE			(APB2_BASE + 0x800U)
#define GPIOB_BASE			(APB2_BASE + 0xC00U)
#define GPIOC_BASE			(APB2_BASE + 0x1000U)
#define GPIOD_BASE			(APB2_BASE + 0x1400U)
#define GPIOE_BASE			(APB2_BASE + 0x1800U)
#define GPIOF_BASE			(APB2_BASE + 0x1C00U)
#define GPIOG_BASE			(APB2_BASE + 0x2000U)
#define USART1_BASE			(APB2_BASE + 0x3800U)

#define USART2_BASE			(APB1_BASE + 0x4400U)
#define USART3_BASE			(APB1_BASE + 0x4800U)
#define UART4_BASE			(APB1_BASE + 0x4C00U)
#define UART5_BASE			(APB1_BASE + 0x5000U)

#define IWDT_BASE			(APB1_BASE + 0x3000U)
#define WWDT_BASE			(APB1_BASE + 0x2C00U)

#define ADC1_BASE			(APB2_BASE + 0x2400U)
#define ADC2_BASE			(APB2_BASE + 0x2800U)
#define ADC3_BASE			(APB2_BASE + 0x3C00U)

#define TIM1_BASE			(APB2_BASE + 0x2C00U)
#define TIM8_BASE			(APB2_BASE + 0x3400U)
#define TIM9_BASE			(APB2_BASE + 0x4C00U)
#define TIM10_BASE			(APB2_BASE + 0x5000U)
#define TIM11_BASE			(APB2_BASE + 0x5400U)

#define TIM2_BASE			(APB1_BASE + 0x0000U)
#define TIM3_BASE			(APB1_BASE + 0x0400U)
#define TIM4_BASE			(APB1_BASE + 0x0800U)
#define TIM5_BASE			(APB1_BASE + 0x0C00U)
#define TIM6_BASE			(APB1_BASE + 0x1000U)
#define TIM7_BASE			(APB1_BASE + 0x1400U)
#define TIM12_BASE			(APB1_BASE + 0x1800U)
#define TIM13_BASE			(APB1_BASE + 0x1C00U)
#define TIM14_BASE			(APB1_BASE + 0x2000U)

#define SPI1_BASE			(APB2_BASE + 0x3000U)
#define SPI2_BASE			(APB1_BASE + 0x3800U)
#define SPI3_BASE			(APB1_BASE + 0x3C00U)

#define I2C1_BASE			(APB1_BASE + 0x5400U)
#define I2C2_BASE			(APB1_BASE + 0x5800U)

#define USB_BASE			(APB1_BASE + 0x5C00U)
#define USB_PMA_BASE			(APB1_BASE + 0x6000U)
#define USB_PMA_SIZE			(512u)

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

typedef struct {
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t OAR1;
	volatile uint32_t OAR2;
	volatile uint32_t DR;
	volatile uint32_t SR1;
	volatile uint32_t SR2;
	volatile uint32_t CCR;
	volatile uint32_t TRISE;
} I2C_Type;

typedef struct {
	volatile uint16_t EP0R;
	volatile uint16_t ep0r_padding;
	volatile uint16_t EP1R;
	volatile uint16_t ep1r_padding;
	volatile uint16_t EP2R;
	volatile uint16_t ep2r_padding;
	volatile uint16_t EP3R;
	volatile uint16_t ep3r_padding;
	volatile uint16_t EP4R;
	volatile uint16_t ep4r_padding;
	volatile uint16_t EP5R;
	volatile uint16_t ep5r_padding;
	volatile uint16_t EP6R;
	volatile uint16_t ep6r_padding;
	volatile uint16_t EP7R;
	volatile uint16_t ep7r_padding;
	volatile uint16_t reserved[16];
	volatile uint16_t CNTR;
	volatile uint16_t cntr_padding;
	volatile uint16_t ISTR;
	volatile uint16_t istr_padding;
	volatile uint16_t FNR;
	volatile uint16_t fnr_padding;
	volatile uint16_t DADDR;
	volatile uint16_t daddr_padding;
	volatile uint16_t BTABLE;
	volatile uint16_t btable_padding;
} USB_Type;

#define DBGMCU				((DBGMCU_Type *)DBGMCU_BASE)

#define SYSCFG				((SYSCFG_Type *)SYSCFG_BASE)
#define EXTI				((EXTI_Type *)EXTI_BASE)
#define RCC				((RCC_Type *)RCC_BASE)
#define FLASH				((FLASH_Type *)FLASH_BASE)

#define GPIOA				((GPIO_Type *)GPIOA_BASE)
#define GPIOB				((GPIO_Type *)GPIOB_BASE)
#define GPIOC				((GPIO_Type *)GPIOC_BASE)
#define GPIOD				((GPIO_Type *)GPIOD_BASE)
#define GPIOE				((GPIO_Type *)GPIOE_BASE)
#define GPIOF				((GPIO_Type *)GPIOF_BASE)
#define GPIOG				((GPIO_Type *)GPIOG_BASE)

#define USART1				((USART_Type *)USART1_BASE)
#define USART2				((USART_Type *)USART2_BASE)
#define USART3				((USART_Type *)USART3_BASE)
#define UART4				((USART_Type *)UART4_BASE)
#define UART5				((USART_Type *)UART5_BASE)

#define IWDT				((IWDT_Type *)IWDT_BASE)

#define ADC1				((ADC_Type *)ADC1_BASE)
#define ADC2				((ADC_Type *)ADC2_BASE)
#define ADC3				((ADC_Type *)ADC3_BASE)

#define TIM1				((TIM_Type *)TIM1_BASE)
#define TIM2				((TIM_Type *)TIM2_BASE)
#define TIM3				((TIM_Type *)TIM3_BASE)
#define TIM4				((TIM_Type *)TIM4_BASE)
#define TIM5				((TIM_Type *)TIM5_BASE)
#define TIM6				((TIM_Type *)TIM6_BASE)
#define TIM7				((TIM_Type *)TIM7_BASE)
#define TIM8				((TIM_Type *)TIM8_BASE)
#define TIM9				((TIM_Type *)TIM9_BASE)
#define TIM10				((TIM_Type *)TIM10_BASE)
#define TIM11				((TIM_Type *)TIM11_BASE)
#define TIM12				((TIM_Type *)TIM12_BASE)
#define TIM13				((TIM_Type *)TIM13_BASE)
#define TIM14				((TIM_Type *)TIM14_BASE)

#define SPI1				((SPI_Type *)SPI1_BASE)
#define SPI2				((SPI_Type *)SPI2_BASE)
#define SPI3				((SPI_Type *)SPI3_BASE)

#define I2C1				((I2C_Type *)I2C1_BASE)
#define I2C2				((I2C_Type *)I2C2_BASE)

#define USB				((USB_Type *)USB_BASE)

#if defined(__cplusplus)
}
#endif

#endif /* STM32F1_H */
