#include "halmcu/delay.h"
#include "halmcu/periph/clk.h"

#define CYCLES_PER_LOOP			2
#define F_10MHZ				10000000U

static void delay_loop(unsigned int count)
{
	__asm__ __volatile__(
			"1:	subs	%[cnt], #1	\n\t"
			"	nop	\n\t"
			"	bne	1b		\n\t"
			: [cnt] "+r" (count) :: "cc");
}

void udelay(unsigned int usec)
{
	/* NOTE: A few additional delay will be added due to cycle calculation
	 * including `clk_get_hclk_frequency()` call. */
	unsigned int hclk_mhz = clk_get_hclk_frequency() / F_10MHZ;
	unsigned int cycles = usec * ((hclk_mhz > 0)? hclk_mhz : 1);
	delay_loop(cycles / CYCLES_PER_LOOP);
}
