#include "abov/hal/wdt.h"
#include "abov/hal/irq.h"
#include "abov/delay.h"

int main(void)
{
	wdt_enable();
	wdt_set_clock_source(CLK_INTERNAL_OSC);
	wdt_set_prescaler(0);
#if defined(ENABLE_WATCHDOG_INTERRUPT)
	wdt_set_interrupt(true);
	irq_enable(IRQ_WDT);
#endif
	wdt_start();
	wdt_set_ms(1000);

	for (int i = 0; i < 10; i++) {
		wdt_feed();
		udelay(500000);
	}

	while (1) {
		/* waiting for watchdog event */
	}

	return 0;
}

#if defined(ENABLE_WATCHDOG_INTERRUPT)
void ISR_WDT(void)
{
	wdt_feed();
}
#endif
