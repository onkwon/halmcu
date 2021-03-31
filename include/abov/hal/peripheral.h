#ifndef ABOV_PERIPHERAL_HAL_H
#define ABOV_PERIPHERAL_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

/** Each peripheral space is defined at 256 intervals.
 * Say there are 3 UARTs, each UART can be accessed with PERIPHERAL_GPIO + n. */
typedef enum {
	PERIPHERAL_BASE			= 0,
	/** Debug peripheral */
	PERIPHERAL_DEBUG		= PERIPHERAL_BASE + 0x0000,
	/** Power peripheral */
	PERIPHERAL_POWER		= PERIPHERAL_BASE + 0x0100,
	/** Watchdog peripheral */
	PERIPHERAL_WDT			= PERIPHERAL_BASE + 0x0200,
	/** GPIO peripheral */
	PERIPHERAL_GPIO			= PERIPHERAL_BASE + 0x0300,
	/** AFIO peripheral */
	PERIPHERAL_AFIO			= PERIPHERAL_BASE + 0x0400,
	/** REMAP peripheral */
	PERIPHERAL_REMAP		= PERIPHERAL_BASE + 0x0500,
	/** Timer peripheral */
	PERIPHERAL_TIMER		= PERIPHERAL_BASE + 0x0600,
	/** PWM peripheral */
	PERIPHERAL_PWM			= PERIPHERAL_BASE + 0x0700,
	/** DMA peripheral */
	PERIPHERAL_DMA			= PERIPHERAL_BASE + 0x0800,
	/** ADC peripheral */
	PERIPHERAL_ADC			= PERIPHERAL_BASE + 0x0900,
	/** DAC peripheral */
	PERIPHERAL_DAC			= PERIPHERAL_BASE + 0x0A00,
	/** UART peripheral */
	PERIPHERAL_UART			= PERIPHERAL_BASE + 0x0B00,
	/** SPI peripheral */
	PERIPHERAL_SPI			= PERIPHERAL_BASE + 0x0C00,
	/** I2C peripheral */
	PERIPHERAL_I2C			= PERIPHERAL_BASE + 0x0D00,
	/** CAN peripheral */
	PERIPHERAL_CAN			= PERIPHERAL_BASE + 0x0E00,
	/** USB peripheral */
	PERIPHERAL_USB			= PERIPHERAL_BASE + 0x0F00,
	/** CRC peripheral */
	PERIPHERAL_CRC			= PERIPHERAL_BASE + 0x1000,
	/** JTAG peripheral */
	PERIPHERAL_JTAG			= PERIPHERAL_BASE + 0x1100,
	/** PMC peripheral */
	PERIPHERAL_PMC			= PERIPHERAL_BASE + 0x1200,
	/** FRT peripheral */
	PERIPHERAL_FRT			= PERIPHERAL_BASE + 0x1300,
	/** USART peripheral */
	PERIPHERAL_USART		= PERIPHERAL_BASE + 0x1400,
	/** MXFAIL peripheral */
	PERIPHERAL_MXFAIL		= PERIPHERAL_BASE + 0x1500,
	/** LVD peripheral */
	PERIPHERAL_LVD			= PERIPHERAL_BASE + 0x1600,
} peripheral_t;

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PERIPHERAL_HAL_H */
