#include "abov/hal/wdt.h"
#include "abov/irq.h"
#include "abov/delay.h"

#define OPTIONAL

int main(void)
{
	wdt_enable();
#if defined(OPTIONAL)
	wdt_set_clock_source(CLK_LSI);
#endif
#if defined(ENABLE_WATCHDOG_INTERRUPT)
	wdt_set_interrupt(true);
	irq_enable(IRQ_WDT);
#endif
	wdt_set_reload_ms(1000);
	wdt_start();

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
