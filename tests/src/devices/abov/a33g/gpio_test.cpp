#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/hal/gpio.h"
#include "a33g.h"

extern "C" {
static struct pcu regPA, regPB, regPC, regPD, regPE, regPF;
static struct gpio regGA, regGB, regGC, regGD, regGE, regGF;

static struct pmu regPmu;

struct pcu * const PCA = &regPA;
struct pcu * const PCB = &regPB;
struct pcu * const PCC = &regPC;
struct pcu * const PCD = &regPD;
struct pcu * const PCE = &regPE;
struct pcu * const PCF = &regPF;

struct gpio * const PA = &regGA;
struct gpio * const PB = &regGB;
struct gpio * const PC = &regGC;
struct gpio * const PD = &regGD;
struct gpio * const PE = &regGE;
struct gpio * const PF = &regGF;

struct pmu * const PMU = &regPmu;
}

TEST_GROUP(gpio) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void)
	{
		memset(PCA, 0, sizeof(*PCA));
		memset(PCB, 0, sizeof(*PCB));
		memset(PCC, 0, sizeof(*PCC));
		memset(PCD, 0, sizeof(*PCD));
		memset(PCE, 0, sizeof(*PCE));
		memset(PCF, 0, sizeof(*PCF));
		memset(PA, 0, sizeof(*PA));
		memset(PB, 0, sizeof(*PB));
		memset(PC, 0, sizeof(*PC));
		memset(PD, 0, sizeof(*PD));
		memset(PE, 0, sizeof(*PE));
		memset(PF, 0, sizeof(*PF));

		PCA->CR = PCB->CR = PCC->CR = PCD->CR = PCE->CR = PCF->CR = 0xFFFFFFFF;
	}
};

TEST(gpio, open_ShouldReturnFalse_WhenUnsupportedGpioGiven) {
	LONGS_EQUAL(false, gpio_open(GPIOF + GPIO_PORT_SIZE, GPIO_MODE_PUSHPULL));
	LONGS_EQUAL(false, gpio_open(GPIOA + 16, GPIO_MODE_PUSHPULL));
}

TEST(gpio, open_ShouldSetCR_WhenPushpullModeGiven) {
	gpio_open(GPIOA, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFFC, PCA->CR);
	gpio_open(GPIOA + 1, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFF0, PCA->CR);
	gpio_open(GPIOA + 2, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFC0, PCA->CR);
	gpio_open(GPIOA + 15, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x3FFFFFC0, PCA->CR);
	gpio_open(GPIOA + 8, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x3FFCFFC0, PCA->CR);
}

TEST(gpio, open_ShouldSetCR_WhenOpendrainModeGiven) {
	gpio_open(GPIOA, GPIO_MODE_OPENDRAIN);
	LONGS_EQUAL(0xFFFFFFFD, PCA->CR);
}

TEST(gpio, open_ShouldSetCR_WhenInputModeGiven) {
	gpio_open(GPIOA, GPIO_MODE_INPUT);
	LONGS_EQUAL(0xFFFFFFFE, PCA->CR);
	gpio_open(GPIOB, GPIO_MODE_INPUT_PULLUP);
	LONGS_EQUAL(0xFFFFFFFE, PCB->CR);
	gpio_open(GPIOC, GPIO_MODE_INPUT_PULLDOWN);
	LONGS_EQUAL(0xFFFFFFFE, PCC->CR);
}

TEST(gpio, open_ShouldSetCR_WhenAnalogModeGiven) {
	gpio_open(GPIOA, GPIO_MODE_ANALOG);
	LONGS_EQUAL(0xFFFFFFFF, PCA->CR);
}

TEST(gpio, open_ShouldSetPullup_WhenInputModeWithPullupGiven) {
	gpio_open(GPIOA, GPIO_MODE_INPUT_PULLUP);
	LONGS_EQUAL(0x1, PCA->PCR);
}

TEST(gpio, open_ShouldSetPulldown_WhenInputModeWithPulldownGiven) {
	gpio_open(GPIOA + 15, GPIO_MODE_INPUT_PULLDOWN);
	LONGS_EQUAL(0x80008000, PCA->PCR);
}

TEST(gpio, open_ShouldEnablePER) {
	gpio_open(GPIOA, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x100, PMU->PER);
	gpio_open(GPIOB + 15, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x300, PMU->PER);
	gpio_open(GPIOC + 8, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x700, PMU->PER);
	gpio_open(GPIOD + 7, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xF00, PMU->PER);
	gpio_open(GPIOE + 4, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x1F00, PMU->PER);
	gpio_open(GPIOF + 14, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x3F00, PMU->PER);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt0Given) {
	gpio_open(GPIOA, GPIO_MODE_ANALOG);
	gpio_set_altfunc(GPIOA, 0);
	LONGS_EQUAL(0, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt1Given) {
	gpio_open(GPIOA + 1, GPIO_MODE_ANALOG);
	gpio_set_altfunc(GPIOA + 1, 1);
	LONGS_EQUAL(4, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt2Given) {
	gpio_open(GPIOA + 15, GPIO_MODE_ANALOG);
	gpio_set_altfunc(GPIOA + 15, 2);
	LONGS_EQUAL(0x80000000, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt3Given) {
	LONGS_EQUAL(1, gpio_open(GPIOA + 8, GPIO_MODE_ANALOG));
	LONGS_EQUAL(1, gpio_set_altfunc(GPIOA + 8, 3));
	LONGS_EQUAL(0x00030000, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldReturnfalse_WhenUnsupportedAltFuncGiven) {
	gpio_open(GPIOA, GPIO_MODE_ANALOG);
	LONGS_EQUAL(0, gpio_set_altfunc(GPIOA, 4));
}

TEST(gpio, close_ShouldSetGpioAnalog) {
	PCA->CR = 0;
	LONGS_EQUAL(1, gpio_close(GPIOA));
	LONGS_EQUAL(0x3, PCA->CR);
}
