#ifndef ABOV_CLK_HAL_H
#define ABOV_CLK_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/hal/peripheral.h"

#if !defined(ABOV_PREFIX)
#define clk_reset				abov_clk_reset
#define clk_enable_peripheral			abov_clk_enable_peripheral
#define clk_disable_peripheral			abov_clk_disable_peripheral
#define clk_enable_source			abov_clk_enable_source
#define clk_disable_source			abov_clk_disable_source
#define clk_set_source				abov_clk_set_source
#define clk_get_source				abov_clk_get_source
#define clk_set_pll_frequency			abov_clk_set_pll_frequency
#define clk_enable_pll				abov_clk_enable_pll
#define clk_disable_pll				abov_clk_disable_pll
#define clk_is_pll_locked			abov_clk_is_pll_locked
#define clk_get_hclk_frequency			abov_clk_get_hclk_frequency
#define clk_get_pclk_frequency			abov_clk_get_pclk_frequency
#define clk_get_frequency			abov_clk_get_frequency
#define clk_set_wdt_source			abov_clk_set_wdt_source
#define clk_enable_outpin			abov_clk_enable_outpin
#define clk_disable_outpin			abov_clk_disable_outpin
#define clk_set_outpin_prescaler		abov_clk_set_outpin_prescaler
#define clk_set_outpin_source			abov_clk_set_outpin_source
#define clk_source_t				abov_clk_source_t
#endif

/** Clock source type */
typedef enum {
	/** Internal oscillator */
	ABOV_CLK_LSI,
	/** Internal 16MHz oscillator */
	ABOV_CLK_HSI,
	/** External oscillator */
	ABOV_CLK_HSE,
	/** External sub oscillator */
	ABOV_CLK_LSE,
	/** PLL */
	ABOV_CLK_PLL,
	/** PLL bypass */
	ABOV_CLK_PLL_BYPASS,
} abov_clk_source_t;

/**
 * Reset CLK unit
 *
 * This function makes CLK unit the reset state.
 */
void abov_clk_reset(void);

/**
 * Enable peripheral clock.
 *
 * :param peri: Peripheral declared as :c:type:`peripheral_t` in `peripheral.h`
 */
void abov_clk_enable_peripheral(peripheral_t peri);
/**
 * Disable peripheral clock
 *
 * :param peri: Peripheral declared as :c:type:`peripheral_t` in `peripheral.h`
 */
void abov_clk_disable_peripheral(peripheral_t peri);

/**
 * Enable clock source
 *
 * :param clk: Clock source.
 */
void abov_clk_enable_source(abov_clk_source_t clk);
/**
 * Disable clock source
 *
 * :param clk: Clock source.
 */
void abov_clk_disable_source(abov_clk_source_t clk);

/** Select main clock source */
void abov_clk_set_source(abov_clk_source_t clk);
/** Get main clock source */
abov_clk_source_t abov_clk_get_source(void);

/** Set frequency */
bool abov_clk_set_pll_frequency(abov_clk_source_t clk, abov_clk_source_t clkin,
		uint32_t hz);
/** Enable PLL */
void abov_clk_enable_pll(void);
/** Disable PLL */
void abov_clk_disable_pll(void);
/** Check if PLL is locked */
bool abov_clk_is_pll_locked(void);

/** Get HCLK frequency in Hz */
uint32_t abov_clk_get_hclk_frequency(void);
/** Get PCLK frequency in Hz */
uint32_t abov_clk_get_pclk_frequency(void);
/** Get clock frequency in Hz */
uint32_t abov_clk_get_frequency(abov_clk_source_t clk);

/** Select watchdog clock source */
void abov_clk_set_wdt_source(abov_clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_CLK_HAL_H */
