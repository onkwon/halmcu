#ifndef ABOV_FLASH_LL_H
#define ABOV_FLASH_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "abov/periph/flash.h"

void flash_ll_set_latency(uint32_t clk);
void flash_ll_lock(void);
void flash_ll_unlock(void);
bool flash_ll_erase_page(uint32_t page_address);
bool flash_ll_erase_all_pages(void);
bool flash_ll_program_word(uintptr_t addr, uint32_t data);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_FLASH_LL_H */
