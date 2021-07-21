#ifndef HALMCU_CLK_LL_H
#define HALMCU_CLK_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/clk.h"

/**
 * @brief Reset CLK unit
 *
 * This function makes CLK unit the reset state.
 */
void clk_ll_reset(void);
void clk_ll_enable_peripheral(periph_t peri);
void clk_ll_disable_peripheral(periph_t peri);
void clk_ll_enable_source(clk_source_t clk);
void clk_ll_disable_source(clk_source_t clk);
void clk_ll_set_source(clk_source_t clk);
clk_source_t clk_ll_get_source(void);
bool clk_ll_set_pll_frequency(clk_source_t clk, clk_source_t clkin, uint32_t hz);
void clk_ll_start_pll(void);
void clk_ll_stop_pll(void);
bool clk_ll_is_pll_locked(void);
uint32_t clk_ll_get_hclk_frequency(void);
uint32_t clk_ll_get_pclk_frequency(void);
uint32_t clk_ll_get_frequency(clk_source_t clk);
clk_source_t clk_ll_get_peripheral_clock_source(periph_t peri);
void clk_ll_set_peripheral_clock_source(periph_t peri, clk_source_t clk);
uint32_t clk_ll_get_peripheral_clock_source_frequency(periph_t peri);
void clk_ll_enable_output(void);
void clk_ll_disable_output(void);
void clk_ll_set_output_prescaler(uint32_t div_factor);
void clk_ll_set_output_source(clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_CLK_LL_H */
