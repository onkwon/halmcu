#ifndef ABOV_DWT_H
#define ABOV_DWT_H

#if defined(__cplusplus)
extern "C" {
#endif

#define DCB_DEMCR			(*(volatile uint32_t *)0xE000EDFC)

#define DWT_CTRL			(*(volatile uint32_t *)0xE0001000)
#define DWT_CYCCNT			(*(volatile uint32_t *)0xE0001004)

/** NOTE: These features are not supported in armv6m. */
#define dwt_enable_cycle_counter()	(DWT_CTRL |= 1UL)
#define dwt_disable_cycle_counter()	(DWT_CTRL &= ~1UL)
#define dwt_clear_cycle_counter()	(DWT_CYCCNT = 0)
#define dwt_get_cycle_counter()		(DWT_CYCCNT)

#define dwt_enable()			(DEMCR |= 1UL << 24)

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_DWT_H */
