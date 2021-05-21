#include "abov/ll/flash.h"

#include "abov/bitop.h"
#include "abov/assert.h"
#include "stm32f1.h"

#define FLASH_KEY1				0x45670123ul
#define FLASH_KEY2				0xCDEF89ABul

enum flash_control_bits {
	BIT_FLASH_PROGRAM			= 0,
	BIT_FLASH_SECTOR_ERASE			= 1,
	BIT_FLASH_MASS_ERASE			= 2,
	BIT_FLASH_OPT_PROGRAM			= 4,
	BIT_FLASH_OPT_BYTE_ERASE		= 5,
	BIT_FLASH_START				= 6,
	BIT_FLASH_LOCK				= 7,
	BIT_FLASH_OPT_BYTE_WRITE_ENABLE		= 9,
	BIT_FLASH_ERR_INT			= 10,
	BIT_FLASH_END_OP_INT			= 12,
};

enum flash_status_bits {
	BIT_FLASH_BUSY				= 0,
	BIT_FLASH_PROG_ERR			= 2,
	BIT_FLASH_WRITE_PROTECTION_ERR		= 4,
	BIT_FLASH_EOP				= 5,
	FLASH_STATUS_MASK			= 0x34,
	FLASH_STATUS_ERROR_MASK			= 0x14,
};

static void clear_flags(void)
{
	bitop_set(&FLASH->SR, FLASH_STATUS_MASK);
}

static uint32_t get_error_flags(void)
{
	return FLASH->SR & FLASH_STATUS_ERROR_MASK;
}

static bool is_busy(void)
{
	return !!(FLASH->SR & (1U << BIT_FLASH_BUSY));
}

static bool is_locked(void)
{
	return !!(FLASH->CR & (1U << BIT_FLASH_LOCK));
}

static void wait_for_last_operation(void)
{
	while (is_busy()) {
		/* waiting */
	}
}

static void unlock_flash(void)
{
	if (is_locked()) {
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}
}

static void lock_flash(void)
{
	bitop_set(&FLASH->CR, BIT_FLASH_LOCK);
}

static void program_half_word(uint32_t addr, uint16_t data)
{
	volatile uint16_t *p = (volatile uint16_t *)(uintptr_t)addr;

	wait_for_last_operation();
	bitop_set(&FLASH->CR, BIT_FLASH_PROGRAM);

	*p = data;

	wait_for_last_operation();
	bitop_clear(&FLASH->CR, BIT_FLASH_PROGRAM);
}

void flash_ll_lock(void)
{
	lock_flash();
}

void flash_ll_unlock(void)
{
	unlock_flash();
}

/*
 * 0 wait :     0 < SYSCLK <= 24MHz
 * 1 wait : 24MHz < SYSCLK <= 48MHz
 * 2 wait : 48MHz < SYSCLK <= 72MHz
 */
void flash_ll_set_latency(uint32_t clk)
{
	assert(clk < 3);
	bitop_clean_set_with_mask(&FLASH->ACR, 0, 7, clk);
}

bool flash_ll_erase_page(uint32_t page_address)
{
	clear_flags();

	wait_for_last_operation();
	bitop_set(&FLASH->CR, BIT_FLASH_SECTOR_ERASE);
	FLASH->AR = page_address;
	bitop_set(&FLASH->CR, BIT_FLASH_START);

	wait_for_last_operation();
	bitop_clear(&FLASH->CR, BIT_FLASH_SECTOR_ERASE);

	return get_error_flags() == 0;
}

bool flash_ll_erase_all_pages(void)
{
	clear_flags();

	wait_for_last_operation();
	bitop_set(&FLASH->CR, BIT_FLASH_MASS_ERASE);
	bitop_set(&FLASH->CR, BIT_FLASH_START);

	wait_for_last_operation();
	bitop_clear(&FLASH->CR, BIT_FLASH_MASS_ERASE);

	return get_error_flags() == 0;
}

bool flash_ll_program_word(uint32_t addr, uint32_t data)
{
	clear_flags();

	program_half_word(addr, (uint16_t)data);
	program_half_word(addr, (uint16_t)(data >> 16));

	return get_error_flags() == 0;
}
