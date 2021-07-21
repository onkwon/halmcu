#ifndef ABOV_PERIPH_H
#define ABOV_PERIPH_H

#if defined(__cplusplus)
extern "C" {
#endif

#define DEFINE_IRQ(n, name) PERIPH_ ## name = (n),
#define RESERVE_IRQ(n)
/** Peripherals enumeration */
typedef enum {
#include IRQ_DEFINES
	PERIPH_MAX,
	PERIPH_UNDEFINED			= PERIPH_MAX,
} periph_t;
#undef RESERVE_IRQ
#undef DEFINE_IRQ

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PERIPH_H */
