#ifndef HALMCU_FLASH_H
#define HALMCU_FLASH_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

void flash_set_latency(uint32_t clk);
void flash_lock(void);
void flash_unlock(void);
bool flash_erase_page(uint32_t page_address);
bool flash_erase_all_pages(void);
bool flash_program_word(uintptr_t addr, uint32_t data);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_FLASH_H */
