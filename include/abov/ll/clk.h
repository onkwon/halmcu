#ifndef ABOV_CLK_LL_H
#define ABOV_CLK_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/peripheral.h"

/** Clock source type */
typedef enum {
	/** Low-speed internal oscillator */
	CLK_LSI,
	/** High-speed internal oscillator */
	CLK_HSI,
	/** Low-speed external oscillator */
	CLK_LSE,
	/** High-speed external oscillator */
	CLK_HSE,
	/** PLL */
	CLK_PLL,
	/** PLL bypass */
	CLK_PLL_BYPASS,
} clk_source_t;

/**
 * Reset CLK unit
 *
 * This function makes CLK unit the reset state.
 */
void clk_reset(void);

/**
 * Enable peripheral clock.
 *
 * :param peri: Peripheral declared as :c:type:`peripheral_t` in `peripheral.h`
 */
void clk_enable_peripheral(peripheral_t peri);
/**
 * Disable peripheral clock
 *
 * :param peri: Peripheral declared as :c:type:`peripheral_t` in `peripheral.h`
 */
void clk_disable_peripheral(peripheral_t peri);

/**
 * Enable clock source
 *
 * :param clk: Clock source.
 */
void clk_enable_source(clk_source_t clk);
/**
 * Disable clock source
 *
 * :param clk: Clock source.
 */
void clk_disable_source(clk_source_t clk);

/** Select main clock source */
void clk_set_source(clk_source_t clk);
/** Get main clock source */
clk_source_t clk_get_source(void);

/** Set frequency */
bool clk_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz);
/** Enable PLL */
void clk_enable_pll(void);
/** Disable PLL */
void clk_disable_pll(void);
/** Check if PLL is locked */
bool clk_is_pll_locked(void);

/** Get HCLK frequency in Hz */
uint32_t clk_get_hclk_frequency(void);
/** Get PCLK frequency in Hz */
uint32_t clk_get_pclk_frequency(void);
/** Get clock frequency in Hz */
uint32_t clk_get_frequency(clk_source_t clk);

/** Get peripheral clock source */
clk_source_t clk_get_peripheral_clock_source(peripheral_t peri);
void clk_set_peripheral_clock_source(peripheral_t peri, clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_CLK_LL_H */
