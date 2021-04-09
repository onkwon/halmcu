#ifndef ABOV_TIMER_HAL_H
#define ABOV_TIMER_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/timer.h"
#include "abov/ll/clk.h"
#include "abov/ll/pwr.h"

struct timer_cfg {
	timer_mode_t mode;
	uint32_t f_hz;
};

void timer_set_clock_source(peripheral_t peri, clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_TIMER_HAL_H */
