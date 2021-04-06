#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/gpio.h"
#include "a33g.h"

extern "C" {
static struct pcu_stub regPA, regPB, regPC, regPD, regPE, regPF;
static struct gpio_stub regGA, regGB, regGC, regGD, regGE, regGF;

static struct pmu_stub regPmu;

struct pcu_stub * const PCA = &regPA;
struct pcu_stub * const PCB = &regPB;
struct pcu_stub * const PCC = &regPC;
struct pcu_stub * const PCD = &regPD;
struct pcu_stub * const PCE = &regPE;
struct pcu_stub * const PCF = &regPF;

struct gpio_stub * const PA = &regGA;
struct gpio_stub * const PB = &regGB;
struct gpio_stub * const PC = &regGC;
struct gpio_stub * const PD = &regGD;
struct gpio_stub * const PE = &regGE;
struct gpio_stub * const PF = &regGF;

struct pmu_stub * const PMU = &regPmu;
}

TEST_GROUP(gpio) {
	void setup(void) {
		set_initial_state();
	}
	void teardown(void) {
	}

	void set_initial_state(void) {
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

		memset(PMU, 0, sizeof(*PMU));
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

TEST(gpio, clear_intr_flag_ShouldWriteValueOneInIsr) {
	gpio_clear_intr_flag(GPIOA);
	LONGS_EQUAL(0x1, PCA->ISR);
	gpio_clear_intr_flag(GPIOA + 15);
	LONGS_EQUAL(0x40000001, PCA->ISR);
}

TEST(gpio, enable_intr_ShouldReturnFalse_WhenUnsupportedGpioGiven) {
	LONGS_EQUAL(0, gpio_enable_intr(GPIOF + GPIO_PORT_SIZE, GPIO_INTR_EDGE_ANY));
	LONGS_EQUAL(0, gpio_enable_intr(GPIOA + 16, GPIO_INTR_EDGE_ANY));
}

TEST(gpio, enable_intr_ShouldSetIcrRegister) {
	gpio_enable_intr(GPIOA, GPIO_INTR_EDGE_ANY);
	LONGS_EQUAL(0x3, PCA->ICR);
	gpio_enable_intr(GPIOA, GPIO_INTR_EDGE_RISING);
	LONGS_EQUAL(0x2, PCA->ICR);
	gpio_enable_intr(GPIOA, GPIO_INTR_EDGE_FALLING);
	LONGS_EQUAL(0x1, PCA->ICR);
	gpio_enable_intr(GPIOA, GPIO_INTR_LEVEL_LOW);
	LONGS_EQUAL(0x1, PCA->ICR);
	gpio_enable_intr(GPIOA, GPIO_INTR_LEVEL_HIGH);
	LONGS_EQUAL(0x2, PCA->ICR);
}

TEST(gpio, disable_intr_ShouldClearIerRegister) {
	gpio_enable_intr(GPIOA, GPIO_INTR_EDGE_ANY);
	gpio_enable_intr(GPIOA + 1, GPIO_INTR_EDGE_ANY);
	gpio_enable_intr(GPIOA + 15, GPIO_INTR_EDGE_ANY);
	LONGS_EQUAL(0xC000000F, PCA->IER);

	gpio_disable_intr(GPIOA);
	LONGS_EQUAL(0xC000000C, PCA->IER);
	gpio_disable_intr(GPIOA + 1);
	LONGS_EQUAL(0xC0000000, PCA->IER);
	gpio_disable_intr(GPIOA + 15);
	LONGS_EQUAL(0x00000000, PCA->IER);
}

TEST(gpio, disable_intr_ShouldReturnFalse_WhenUnsupportedGpioGiven) {
	LONGS_EQUAL(0, gpio_disable_intr(GPIOF + GPIO_PORT_SIZE));
	LONGS_EQUAL(0, gpio_disable_intr(GPIOA + 16));
}

TEST(gpio, write_ShouldSetSpecificPin) {
	gpio_write(GPIOA, 0);
	LONGS_EQUAL(0x10000, PA->SRR);
	gpio_write(GPIOA, 1);
	LONGS_EQUAL(0x1, PA->SRR);
	gpio_write(GPIOB + 15, 0);
	LONGS_EQUAL(0x80000000, PB->SRR);
	gpio_write(GPIOB + 15, 1);
	LONGS_EQUAL(0x8000, PB->SRR);
}

TEST(gpio, write_ShouldDoNothing_WhenUnsupportedGpioGiven) {
	gpio_write(GPIOF + GPIO_PORT_SIZE, 1);
	gpio_write(GPIOA + 16, 1);
}

TEST(gpio, read_ShouldReturnCurrentPinState) {
	LONGS_EQUAL(0, gpio_read(GPIOA));
	PA->IDR = 1;
	LONGS_EQUAL(1, gpio_read(GPIOA));
	PB->IDR = 0xA5;
	LONGS_EQUAL(1, gpio_read(GPIOB + 0));
	LONGS_EQUAL(0, gpio_read(GPIOB + 1));
	LONGS_EQUAL(1, gpio_read(GPIOB + 2));
	LONGS_EQUAL(0, gpio_read(GPIOB + 3));
	LONGS_EQUAL(0, gpio_read(GPIOB + 4));
	LONGS_EQUAL(1, gpio_read(GPIOB + 5));
	LONGS_EQUAL(0, gpio_read(GPIOB + 6));
	LONGS_EQUAL(1, gpio_read(GPIOB + 7));
}

TEST(gpio, read_ShouldReturnNegative_WhenUnsupportedGpioGiven) {
	LONGS_EQUAL(-1, gpio_read(GPIOF + GPIO_PORT_SIZE));
	LONGS_EQUAL(-1, gpio_read(GPIOA + 16));
}

TEST(gpio, write_port_ShouldWriteValue) {
	gpio_write_port(GPIOA, 0xaa55);
	LONGS_EQUAL(0xaa55, PA->ODR);
	gpio_write_port(GPIOF, 0x1234);
	LONGS_EQUAL(0x1234, PF->ODR);
}

TEST(gpio, read_port_ShouldReturnCurrentState) {
	PA->IDR = 0xA5A5;
	LONGS_EQUAL(0xA5A5, gpio_read_port(GPIOA));
}

TEST(gpio, enable_port_ShouldSetPmuPer) {
	gpio_enable_port(GPIOA);
	LONGS_EQUAL(0x100, PMU->PER);
	gpio_enable_port(GPIOF);
	LONGS_EQUAL(0x2100, PMU->PER);
}

TEST(gpio, disable_port_ShouldClearPmuPer) {
	PMU->PER = 0x1200;
	gpio_disable_port(GPIOB);
	LONGS_EQUAL(0x1000, PMU->PER);
	gpio_disable_port(GPIOE);
	LONGS_EQUAL(0, PMU->PER);
}
