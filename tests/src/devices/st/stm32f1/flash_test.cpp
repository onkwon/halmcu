#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/flash.h"
#include "stm32f1.h"

extern "C" {
static FLASH_Type flash_reg;
FLASH_Type * const FLASH = &flash_reg;
}

TEST_GROUP(FLASH) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(FLASH, 0, sizeof(*FLASH));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(FLASH, lock_ShouldLockFlash) {
	flash_ll_lock();
	LONGS_EQUAL(0x80, FLASH->CR);
}

TEST(FLASH, unlock_ShouldUnlockFlash) {
	flash_ll_unlock();
	LONGS_EQUAL(0, FLASH->KEYR); // already unlocked
	flash_ll_lock();
	flash_ll_unlock();
	LONGS_EQUAL(0xCDEF89ABul, FLASH->KEYR);
}

TEST(FLASH, set_latency_ShouldSetACR) {
	flash_ll_set_latency(2);
	LONGS_EQUAL(2, FLASH->ACR);
	flash_ll_set_latency(1);
	LONGS_EQUAL(1, FLASH->ACR);
	flash_ll_set_latency(0);
	LONGS_EQUAL(0, FLASH->ACR);
}

TEST(FLASH, erase_page_ShouldReturnTrue_WhenSuccessToErase) {
	mock().expectNCalls(3, "bitop_set").ignoreOtherParameters();
	mock().expectOneCall("bitop_clear").ignoreOtherParameters();

	LONGS_EQUAL(1, flash_ll_erase_page(0x08003456));
	LONGS_EQUAL(0x08003456, FLASH->AR);
}

TEST(FLASH, erase_all_pages_ShouldReturnTrue_WhenSuccessToErase) {
	mock().expectNCalls(3, "bitop_set").ignoreOtherParameters();
	mock().expectOneCall("bitop_clear").ignoreOtherParameters();
	LONGS_EQUAL(1, flash_ll_erase_all_pages());
}
