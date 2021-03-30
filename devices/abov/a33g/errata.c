#include "abov/errata.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"
#include "abov/system.h"

#define PMU_LEGACY_MODE					(0x0001UL << 31)
#define PMU_LEGACY_MODE_NORMAL				(0x0000UL << 31)

#define PMU_LEGACY_NORMAL_STEP1				(0xDADAUL << 0)
#define PMU_LEGACY_NORMAL_STEP2				(0xDAADUL << 0)

void errata_1(void)
{
	uint32_t val;

	PMULEGACY->LEGACY = PMU_LEGACY_NORMAL_STEP1;
	PMULEGACY->LEGACY = PMU_LEGACY_NORMAL_STEP2;

	enable_irq();

	do {
		val = PMULEGACY->LEGACY;
	} while ((val & PMU_LEGACY_MODE) != PMU_LEGACY_MODE_NORMAL);

	disable_irq();
}
