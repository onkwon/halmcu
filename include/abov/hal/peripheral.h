#ifndef ABOV_PERIPHERAL_HAL_H
#define ABOV_PERIPHERAL_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef enum {
	PERIPHERAL_BASE			= 0,
	PERIPHERAL_DEBUG		= PERIPHERAL_BASE + 0x0000,
	PERIPHERAL_POWER		= PERIPHERAL_BASE + 0x0100,
	PERIPHERAL_WDT			= PERIPHERAL_BASE + 0x0200,
	PERIPHERAL_GPIO			= PERIPHERAL_BASE + 0x0300,
	PERIPHERAL_AFIO			= PERIPHERAL_BASE + 0x0400,
	PERIPHERAL_REMAP		= PERIPHERAL_BASE + 0x0500,
	PERIPHERAL_TIMER		= PERIPHERAL_BASE + 0x0600,
	PERIPHERAL_PWM			= PERIPHERAL_BASE + 0x0700,
	PERIPHERAL_DMA			= PERIPHERAL_BASE + 0x0800,
	PERIPHERAL_ADC			= PERIPHERAL_BASE + 0x0900,
	PERIPHERAL_DAC			= PERIPHERAL_BASE + 0x0A00,
	PERIPHERAL_UART			= PERIPHERAL_BASE + 0x0B00,
	PERIPHERAL_SPI			= PERIPHERAL_BASE + 0x0C00,
	PERIPHERAL_I2C			= PERIPHERAL_BASE + 0x0D00,
	PERIPHERAL_CAN			= PERIPHERAL_BASE + 0x0E00,
	PERIPHERAL_USB			= PERIPHERAL_BASE + 0x0F00,
	PERIPHERAL_CRC			= PERIPHERAL_BASE + 0x1000,
	PERIPHERAL_JTAG			= PERIPHERAL_BASE + 0x1100,
	PERIPHERAL_PMC			= PERIPHERAL_BASE + 0x1200,
	PERIPHERAL_FRT			= PERIPHERAL_BASE + 0x1300,
	PERIPHERAL_USART		= PERIPHERAL_BASE + 0x1400,
	PERIPHERAL_MXFAIL		= PERIPHERAL_BASE + 0x1500,
	PERIPHERAL_LVD			= PERIPHERAL_BASE + 0x1600,
} peripheral_t;

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PERIPHERAL_HAL_H */
