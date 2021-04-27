#include "abov/ll/wdt.h"

#include <assert.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "stm32f1.h"

static bool is_prescaler_busy(void)
{
	return IWDT->SR & 1U; /* PVU */
}

static bool is_reload_busy(void)
{
	return (IWDT->SR >> 1) & 1U; /* RVU */
}

static uint32_t get_prescaler_value_from_divisor(uint32_t divisor)
{
	uint32_t i = 0;

	while (divisor != 0) {
		divisor >>= 1;
		i++;
	}

	return i - 3;
}

void wdt_set_prescaler(uint32_t div_factor)
{
	assert(is_pwr2(div_factor) && div_factor >= 4 && div_factor <= 256);

	uint32_t val = get_prescaler_value_from_divisor(div_factor);

	while (is_prescaler_busy()) {
		/* waiting */
	}

	IWDT->KR = 0x5555;
	IWDT->PR = val;
}

uint32_t wdt_get_prescaler(void)
{
	uint32_t prescaler = IWDT->PR;

	if (prescaler == 0) {
		return 4;
	}

	return 1U << (prescaler + 2);
}

void wdt_set_reload(uint32_t timeout)
{
	if (timeout > 0xfff) {
		timeout = 0xfff;
	}

	while (is_reload_busy()) {
		/* waiting */
	}

	IWDT->KR = 0x5555;
	IWDT->RLP = timeout;
}

uint32_t wdt_get_reload(void)
{
	return IWDT->RLP;
}

void wdt_feed(void)
{
	IWDT->KR = 0xAAAA;
}

void wdt_start(void)
{
	IWDT->KR = 0xCCCC;
}

void wdt_set_debug_stop_mode(bool enable)
{
	bitop_clean_set_with_mask(&DBGMCU->CR, 8/*DBG_IWDGSTOP*/, 1, enable);
}

clk_source_t wdt_get_clock_source(void)
{
	return CLK_LSI;
}
