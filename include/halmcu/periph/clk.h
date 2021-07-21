#ifndef HALMCU_CLK_H
#define HALMCU_CLK_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "halmcu/periph/periph.h"

/** Clock source type */
typedef enum {
	CLK_LSI,	/**< Low-speed internal oscillator */
	CLK_HSI,	/**< High-speed internal oscillator */
	CLK_LSE,	/**< Low-speed external oscillator */
	CLK_HSE,	/**< High-speed external oscillator */
	CLK_PLL,	/**< PLL */
	CLK_PLL_BYPASS,	/**< PLL bypass */
} clk_source_t;

/**
 * @brief Reset CLK unit
 *
 * This function makes CLK unit the reset state.
 */
void clk_reset(void);
/**
 * @brief Enable peripheral clock.
 *
 * @param[in] peri enumerated in @ref periph_t
 */
void clk_enable_peripheral(periph_t peri);
/**
 * @brief Disable peripheral clock
 *
 * @param[in] peri enumerated in @ref periph_t
 */
void clk_disable_peripheral(periph_t peri);
/**
 * @brief Enable clock source
 *
 * @param[in] clk Clock source.
 */
void clk_enable_source(clk_source_t clk);
/**
 * @brief Disable clock source
 *
 * @param[in] clk Clock source.
 */
void clk_disable_source(clk_source_t clk);

/** @brief Select main clock source */
void clk_set_source(clk_source_t clk);
/** @brief Get main clock source */
clk_source_t clk_get_source(void);

/** @brief Set frequency */
bool clk_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz);
/** @brief Enable PLL */
void clk_start_pll(void);
/** @brief Disable PLL */
void clk_stop_pll(void);
/** @brief Check if PLL is locked */
bool clk_is_pll_locked(void);

/** @brief Get processor clock frequency in Hz */
uint32_t clk_get_hclk_frequency(void);
/** @brief Get PCLK frequency in Hz */
uint32_t clk_get_pclk_frequency(void);
/** @brief Get clock frequency in Hz */
uint32_t clk_get_frequency(clk_source_t clk);

/** @brief Get peripheral clock source */
clk_source_t clk_get_peripheral_clock_source(periph_t peri);
/***/
void clk_set_peripheral_clock_source(periph_t peri, clk_source_t clk);
uint32_t clk_get_peripheral_clock_source_frequency(periph_t peri);

/***/
void clk_enable_output(void);
/***/
void clk_disable_output(void);
/***/
void clk_set_output_prescaler(uint32_t div_factor);
/***/
void clk_set_output_source(clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_CLK_H */
