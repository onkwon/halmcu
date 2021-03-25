#include "../arch/arm/armv7-m/nvic.h"
#include <assert.h>
#include "abov/compiler.h"

ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_lvdfail(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_mxoscfail(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_wdt(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_frt(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer0(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer1(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer2(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer3(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer4(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer5(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer6(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer7(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer8(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_timer9(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_mckfail(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpioa(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpiob(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpioc(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpiod(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpioe(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_gpiof(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm0(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm1(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm2(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm3(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm4(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm5(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm6(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_pwm7(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_spi0(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_spi1(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_i2c0(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_i2c1(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_uart0(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_uart1(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_uart2(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_uart3(void);
ABOV_WEAK ABOV_ALIAS("default_irq_handler") void ISR_adc(void);

ABOV_WEAK
ABOV_USED
ABOV_SECTION(".external_isr_vectors")
const nvic_vector_t external_isr_vectors[] = {
	ISR_lvdfail,
	ISR_mxoscfail,
	0, /* reserved */
	ISR_wdt,
	ISR_frt,
	ISR_timer0,
	ISR_timer1,
	ISR_timer2,
	ISR_timer3,
	ISR_timer4,
	ISR_timer5,
	ISR_timer6,
	ISR_timer7,
	ISR_timer8,
	ISR_timer9,
	ISR_mckfail,
	ISR_gpioa,
	ISR_gpiob,
	ISR_gpioc,
	ISR_gpiod,
	ISR_gpioe,
	ISR_gpiof,
	0, /* reserved */
	0, /* reserved */
	ISR_pwm0,
	ISR_pwm1,
	ISR_pwm2,
	ISR_pwm3,
	ISR_pwm4,
	ISR_pwm5,
	ISR_pwm6,
	ISR_pwm7,
	ISR_spi0,
	ISR_spi1,
	0, /* reserved */
	0, /* reserved */
	ISR_i2c0,
	ISR_i2c1,
	ISR_uart0,
	ISR_uart1,
	ISR_uart2,
	ISR_uart3,
	0, /* reserved */
	ISR_adc,
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
	0, /* reserved */
};

ABOV_WEAK ABOV_USED void default_irq_handler(void)
{
	assert(0);
}
