#include "abov/system.h"
#include "abov/irq.h"
#include "abov/delay.h"

#include "abov/ll/i2c.h"
#include "abov/ll/pwr.h"
#include "abov/ll/clk.h"
#include "abov/ll/gpio.h"

#define I2C_SCL				(GPIOB + 14)
#define I2C_SDA				(GPIOB + 15)

static void myi2c_gpio_init(void)
{
	gpio_open(I2C_SCL, GPIO_MODE_OPENDRAIN);
	gpio_open(I2C_SDA, GPIO_MODE_OPENDRAIN);
	gpio_set_altfunc(I2C_SCL, 1);
	gpio_set_altfunc(I2C_SDA, 1);
}

static void myi2c_init(void)
{
	myi2c_gpio_init();

	pwr_enable_peripheral(PERI_I2C0);
	clk_enable_peripheral(PERI_I2C0);
	i2c_reset(PERI_I2C0);
	i2c_set_frequency(PERI_I2C0, 100000, clk_get_pclk_frequency());

#if defined(I2C_INTERRUPT)
	i2c_enable_irq(PERI_I2C0);
	irq_enable(PERI_TO_IRQ(PERI_I2C0));
#endif
}

int main(void)
{
	myi2c_init();

	/* get l3gd20 whoami */
	i2c_start(PERI_I2C0, 0x69, 0);
	i2c_write_byte(PERI_I2C0, 0xf);
	i2c_start(PERI_I2C0, 0x69, 1);
	int rxd = i2c_read_byte(PERI_I2C0, false);
	i2c_stop(PERI_I2C0);
	(void)rxd; /* value of whoami register */

	/* change power mode to normal */
	i2c_start(PERI_I2C0, 0x69, 0);
	i2c_write_byte(PERI_I2C0, 0x20);
	i2c_write_byte(PERI_I2C0, 0xf);
	i2c_stop(PERI_I2C0);

	while (1) {
		/* hang */
	}

	return 0;
}

void ISR_I2C0(void)
{
}
