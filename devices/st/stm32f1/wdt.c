#include "halmcu/periph/wdt.h"

#include "halmcu/bitop.h"
#include "halmcu/compiler.h"
#include "halmcu/assert.h"
#include "stm32f1.h"

#define MAX_RELOAD				(0x1000 - 1)

static bool is_prescaler_busy(void)
{
	return IWDT->SR & 1U; /* PVU */
}

static bool is_reload_busy(void)
{
	return (IWDT->SR >> 1) & 1U; /* RVU */
}

HALMCU_STATIC_ASSERT(__builtin_ffs(1) == 1, "");
static uint32_t get_prescaler_value_from_divisor(uint32_t divisor)
{
	return (uint32_t)(__builtin_ffs((int)divisor) - 3);
}

static void set_reload(uint32_t timeout)
{
	if (timeout > MAX_RELOAD) {
		timeout = MAX_RELOAD;
	}

	while (is_reload_busy()) {
		/* waiting */
	}

	IWDT->KR = 0x5555;
	IWDT->RLP = timeout;
}

static void set_prescaler(uint32_t value)
{
	while (is_prescaler_busy()) {
		/* waiting */
	}

	IWDT->KR = 0x5555;
	IWDT->PR = value;
}

void wdt_set_prescaler(uint32_t div_factor)
{
	assert(is_pwr2(div_factor) && div_factor >= 4 && div_factor <= 256);
	uint32_t val = get_prescaler_value_from_divisor(div_factor);
	set_prescaler(val);
}

uint32_t wdt_get_prescaler(void)
{
	uint32_t prescaler = IWDT->PR & 7;
	return 1U << (prescaler + 2);
}

void wdt_set_reload(uint32_t timeout)
{
	set_reload(timeout);
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

void wdt_set_reload_ms(uint32_t period_ms)
{
	uint32_t lsi = 40000;
	uint32_t prescaler = 0;
	uint32_t ticks = MAX_RELOAD;

	while (prescaler <= 6) {
		uint32_t clk = lsi >> (prescaler + 2);
		ticks = clk * period_ms / 1000;
		if (ticks <= MAX_RELOAD) {
			break;
		}
		prescaler++;
	}

	set_prescaler(prescaler);
	set_reload(ticks);
}
