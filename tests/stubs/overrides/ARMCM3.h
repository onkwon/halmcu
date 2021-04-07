#ifndef ARMCM3_OVERRIDE_H
#define ARMCM3_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

#define SysTick_LOAD_RELOAD_Msk			((1U << 24) - 1)
#define SysTick_CTRL_CLKSOURCE_Pos		2U
#define SysTick_CTRL_TICKINT_Msk		2U
#define SysTick_CTRL_ENABLE_Msk			1U

struct systick {
	uint32_t CTRL;
	uint32_t LOAD;
	uint32_t VAL;
	uint32_t CALIB;
};

extern struct systick * const SysTick;

#if defined(__cplusplus)
}
#endif

#endif
