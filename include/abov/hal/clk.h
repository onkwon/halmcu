#ifndef ABOV_CLK_HAL_H
#define ABOV_CLK_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/clk.h"

bool clk_init(clk_source_t clock_source, uint32_t target_hz);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_CLK_HAL_H */
