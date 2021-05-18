#ifndef ABOV_PWR_LL_H
#define ABOV_PWR_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/periph/pwr.h"

void pwr_ll_reset(void);
void pwr_ll_reboot(void);
uint32_t pwr_ll_get_reboot_source(void);
void pwr_ll_clear_reboot_source(uint32_t bitmask);
void pwr_ll_set_mode(pwr_mode_t sleep_mode);
void pwr_ll_set_wakeup_source(periph_t peri);
void pwr_ll_clear_wakeup_source(periph_t peri);
uint32_t pwr_ll_get_wakeup_source(void);
void pwr_ll_enable_peripheral(periph_t peri);
void pwr_ll_disable_peripheral(periph_t peri);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_PWR_LL_H */
