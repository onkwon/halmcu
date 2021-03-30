#ifndef ABOV_CLK_HAL_H
#define ABOV_CLK_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include "abov/hal/peripheral.h"

typedef enum {
	CLK_INTERNAL_OSC,
	CLK_INTERNAL_OSC_16MHZ,
	CLK_EXTERNAL_OSC,
	CLK_EXTERNAL_OSC_SUB,
} clk_source_t;

void clk_enable_peripheral(peripheral_t peri);
void clk_disable_peripheral(peripheral_t peri);

void clk_enable_source(clk_source_t clk);
void clk_disable_source(clk_source_t clk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_CLK_HAL_H */
