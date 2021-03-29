#ifndef A33G_OVERRIDE_H
#define A33G_OVERRIDE_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>

struct wdt {
	uint32_t LR;
	uint32_t CVR;
	uint32_t CON;
};

extern struct wdt * const WDT;

#if defined(__cplusplus)
}
#endif

#endif
