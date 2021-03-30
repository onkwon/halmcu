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

struct pmu {
	uint32_t IDR;
	uint32_t MR;
	uint32_t CFG;
	uint32_t WSER;
	uint32_t WSSR;
	uint32_t RSER;
	uint32_t RSSR;
	uint32_t PRER;
	uint32_t PER;
	uint32_t PCCR;
	uint32_t CCR;
	uint32_t CMR;
	uint32_t MCMR;
	uint32_t BCCR;
	uint32_t PCSR;
	uint32_t COR;
	uint32_t PLLCON;
	uint32_t LVDCON;
	uint32_t VDCCON;
	uint32_t IOSC16TRIM;
	uint32_t EOSCCON;
	uint32_t EXTMODER;
};

extern struct wdt * const WDT;
extern struct pmu * const PMU;

#if defined(__cplusplus)
}
#endif

#endif
