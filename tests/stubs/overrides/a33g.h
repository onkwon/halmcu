#ifndef A33G_OVERRIDE_H
#define A33G_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

struct wdt_stub {
	uint32_t LR;
	uint32_t CVR;
	uint32_t CON;
};

struct pmu_stub {
	uint32_t IDR;
	uint32_t MR;
	uint32_t CFG;
	uint32_t WSER;
	uint32_t WSSR;
	uint32_t RSER;
	uint32_t RSSR;
	uint32_t PRER;
	uint32_t PER;
	uint32_t PCCR;
	uint32_t CCR;
	uint32_t CMR;
	uint32_t MCMR;
	uint32_t BCCR;
	uint32_t PCSR;
	uint32_t COR;
	uint32_t PLLCON;
	uint32_t LVDCON;
	uint32_t VDCCON;
	uint32_t IOSC16TRIM;
	uint32_t EOSCCON;
	uint32_t EXTMODER;
};

struct pcu_stub {
	uint32_t MR;
	uint32_t CR;
	uint32_t PCR;
	uint32_t DER;
	uint32_t IER;
	uint32_t ISR;
	uint32_t ICR;
	uint32_t DPR;
};

struct gpio_stub {
	uint32_t ODR;
	uint32_t IDR;
	uint32_t SRR;
};

struct uart_stub {
	uint32_t RBR;
	uint32_t THR;
	uint32_t DLL;
	uint32_t IER;
	uint32_t DLM;
	uint32_t FCR;
	uint32_t IIR;
	uint32_t LCR;
	uint32_t LSR;
	uint32_t SCR;
	uint32_t BFR;
	uint32_t DTR;
};

struct timer_stub {
	uint32_t CON;
	uint32_t CMD;
	uint32_t GRA;
	uint32_t GRB;
	uint32_t PRS;
	uint32_t CNT;
};

struct i2c_stub {
	uint32_t DR;
	uint32_t SR;
	uint32_t SAR;
	uint32_t CR;
	uint32_t SCLL;
	uint32_t SCLH;
	uint32_t SDH;
};

struct adc_stub {
	uint32_t CR;
	uint32_t MR;
	uint32_t DR;
	uint32_t TEST;
};

struct spi_stub {
	uint32_t RDR;
	uint32_t TDR;
	uint32_t CR;
	uint32_t SR;
	uint32_t BR;
	uint32_t EN;
	uint32_t LR;
};

typedef struct pcu_stub PCU_Type;
typedef struct gpio_stub GPIO_Type;
typedef struct uart_stub UART_Type;
typedef struct timer_stub TIMER_Type;
typedef struct i2c_stub I2C_Type;
typedef struct adc_stub ADC_Type;
typedef struct spi_stub SPI_Type;

extern struct wdt_stub * const WDT;
extern struct pmu_stub * const PMU;

extern struct pcu_stub * const PCA;
extern struct pcu_stub * const PCB;
extern struct pcu_stub * const PCC;
extern struct pcu_stub * const PCD;
extern struct pcu_stub * const PCE;
extern struct pcu_stub * const PCF;

extern struct gpio_stub * const PA;
extern struct gpio_stub * const PB;
extern struct gpio_stub * const PC;
extern struct gpio_stub * const PD;
extern struct gpio_stub * const PE;
extern struct gpio_stub * const PF;

extern struct uart_stub * const UART0;
extern struct uart_stub * const UART1;
extern struct uart_stub * const UART2;
extern struct uart_stub * const UART3;

extern uintptr_t T0_BASE;

extern struct i2c_stub * const I2C0;
extern struct i2c_stub * const I2C1;

extern struct adc_stub * const ADC;

extern struct spi_stub * const SPI0;
extern struct spi_stub * const SPI1;

#if defined(__cplusplus)
}
#endif

#endif
