#ifndef HALMCU_CLK_HAL_H
#define HALMCU_CLK_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/clk.h"

bool clk_init(clk_source_t clock_source, uint32_t target_hz);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_CLK_HAL_H */
