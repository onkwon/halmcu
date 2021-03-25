#include "abov/sys/init.h"
#include "../../cmsis.h"

#define FULL_ACCESS			3UL
#define CP10				(1UL << 20)
#define CP11				(1UL << 22)

void sys_early_init(void)
{
	/* Enable FPU */
	SCB->CPACR |= (CP10 | CP11) * FULL_ACCESS;
}
