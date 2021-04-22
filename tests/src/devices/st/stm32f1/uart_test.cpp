#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/uart.h"
#include "stm32f1.h"
#include "assert.h"

extern "C" {
static USART_Type reg1, reg2, reg3, reg4, reg5;
USART_Type * const USART1 = &reg1;
USART_Type * const USART2 = &reg2;
USART_Type * const USART3 = &reg3;
USART_Type * const UART4 = &reg4;
USART_Type * const UART5 = &reg5;
}

void assert_override(void) {
	mock().actualCall(__func__);
}

TEST_GROUP(UART) {
	void setup(void) {
		mock().ignoreOtherCalls();

		memset(USART1, 0, sizeof(*USART1));
		memset(USART2, 0, sizeof(*USART2));
		memset(USART3, 0, sizeof(*USART3));
		memset(UART4, 0, sizeof(*UART4));
		memset(UART5, 0, sizeof(*UART5));
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
	}
};

TEST(UART, t) {
}
