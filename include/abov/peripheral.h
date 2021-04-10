#ifndef ABOV_PERIPHERAL_H
#define ABOV_PERIPHERAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFINE_IRQ(n, name) PERI_ ## name = (n),
#define RESERVE_IRQ(n)
/** Peripherals enumeration */
typedef enum {
#include IRQ_DEFINES
	PERI_JTAG,
	PERI_PMC,
	PERI_MAX,
	PERI_UNDEFINED			= PERI_MAX,
} peripheral_t;
#undef RESERVE_IRQ
#undef DEFINE_IRQ

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PERIPHERAL_H */
