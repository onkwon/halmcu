#include "CppUTest/TestHarness.h"
#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport.h"

#include <string.h>

#include "abov/ll/gpio.h"
#include "a33g.h"
#include "assert.h"

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

void assert_override(void) {
	mock().actualCall(__func__);
}

TEST_GROUP(gpio) {
	void setup(void) {
		mock().ignoreOtherCalls();

		set_initial_state();
	}
	void teardown(void) {
		mock().checkExpectations();
		mock().clear();
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

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt0Given) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_ANALOG);
	gpio_set_altfunc(PERI_GPIOA, 0, 0);
	LONGS_EQUAL(0, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt1Given) {
	gpio_set_mode(PERI_GPIOA, 1, GPIO_MODE_ANALOG);
	gpio_set_altfunc(PERI_GPIOA, 1, 1);
	LONGS_EQUAL(4, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt2Given) {
	gpio_set_mode(PERI_GPIOA, 15, GPIO_MODE_ANALOG);
	gpio_set_altfunc(PERI_GPIOA, 15, 2);
	LONGS_EQUAL(0x80000000, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldSetMr_WhenAlt3Given) {
	gpio_set_mode(PERI_GPIOA, 8, GPIO_MODE_ANALOG);
	gpio_set_altfunc(PERI_GPIOA, 8, 3);
	LONGS_EQUAL(0x00030000, PCA->MR);
}

TEST(gpio, set_altfunc_ShouldCauseAssert_WhenUnsupportedAltFuncGiven) {
	mock().expectOneCall("assert_override");
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_ANALOG);
	gpio_set_altfunc(PERI_GPIOA, 0, 4);
}

TEST(gpio, clear_event_ShouldWriteValueOneInIsr) {
	gpio_clear_event(PERI_GPIOA, 0);
	LONGS_EQUAL(0x1, PCA->ISR);
	gpio_clear_event(PERI_GPIOA, 15);
	LONGS_EQUAL(0x40000001, PCA->ISR);
}

TEST(gpio, enable_irq_ShouldCauseAssert_WhenUnsupportedGpioGiven) {
	mock().expectOneCall("assert_override");
	gpio_enable_irq(PERI_GPIOA, 16, GPIO_IRQ_EDGE_ANY);
}
TEST(gpio, enable_irq_ShouldSetIcrRegister) {
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_EDGE_ANY);
	LONGS_EQUAL(0x3, PCA->ICR);
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_EDGE_RISING);
	LONGS_EQUAL(0x2, PCA->ICR);
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_EDGE_FALLING);
	LONGS_EQUAL(0x1, PCA->ICR);
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_LEVEL_LOW);
	LONGS_EQUAL(0x1, PCA->ICR);
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_LEVEL_HIGH);
	LONGS_EQUAL(0x2, PCA->ICR);
}

TEST(gpio, disable_irq_ShouldClearIerRegister) {
	gpio_enable_irq(PERI_GPIOA, 0, GPIO_IRQ_EDGE_ANY);
	gpio_enable_irq(PERI_GPIOA, 1, GPIO_IRQ_EDGE_ANY);
	gpio_enable_irq(PERI_GPIOA, 15, GPIO_IRQ_EDGE_ANY);
	LONGS_EQUAL(0xC000000F, PCA->IER);

	gpio_disable_irq(PERI_GPIOA, 0);
	LONGS_EQUAL(0xC000000C, PCA->IER);
	gpio_disable_irq(PERI_GPIOA, 1);
	LONGS_EQUAL(0xC0000000, PCA->IER);
	gpio_disable_irq(PERI_GPIOA, 15);
	LONGS_EQUAL(0x00000000, PCA->IER);
}
TEST(gpio, disable_irq_ShouldCauseAssert_WhenUnsupportedGpioGiven) {
	mock().expectOneCall("assert_override");
	gpio_disable_irq(PERI_GPIOA, 16);
}

TEST(gpio, write_ShouldSetSpecificPin) {
	gpio_write(PERI_GPIOA, 0, 0);
	LONGS_EQUAL(0x10000, PA->SRR);
	gpio_write(PERI_GPIOA, 0, 1);
	LONGS_EQUAL(0x1, PA->SRR);
	gpio_write(PERI_GPIOB, 15, 0);
	LONGS_EQUAL(0x80000000, PB->SRR);
	gpio_write(PERI_GPIOB, 15, 1);
	LONGS_EQUAL(0x8000, PB->SRR);
}

TEST(gpio, write_ShouldCauseAssert_WhenUnsupportedGpioGiven) {
	mock().expectOneCall("assert_override");
	gpio_write(PERI_GPIOA, 16, 1);
}

TEST(gpio, read_ShouldReturnCurrentPinState) {
	LONGS_EQUAL(0, gpio_read(PERI_GPIOA, 0));
	PA->IDR = 1;
	LONGS_EQUAL(1, gpio_read(PERI_GPIOA, 0));
	PB->IDR = 0xA5;
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 0));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 1));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 2));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 3));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 4));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 5));
	LONGS_EQUAL(0, gpio_read(PERI_GPIOB, 6));
	LONGS_EQUAL(1, gpio_read(PERI_GPIOB, 7));
}

TEST(gpio, read_ShouldCauseAssert_WhenUnsupportedGpioGiven) {
	mock().expectOneCall("assert_override");
	gpio_read(PERI_GPIOA, 16);
}

TEST(gpio, write_port_ShouldWriteValue) {
	gpio_write_port(PERI_GPIOA, 0xaa55);
	LONGS_EQUAL(0xaa55, PA->ODR);
	gpio_write_port(PERI_GPIOF, 0x1234);
	LONGS_EQUAL(0x1234, PF->ODR);
}

TEST(gpio, read_port_ShouldReturnCurrentState) {
	PA->IDR = 0xA5A5;
	LONGS_EQUAL(0xA5A5, gpio_read_port(PERI_GPIOA));
}

TEST(gpio, enable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_disable_port(PERI_UART1);
}
TEST(gpio, enable_port_ShouldSetPmuPer) {
	gpio_enable_port(PERI_GPIOA);
	LONGS_EQUAL(0x100, PMU->PER);
	gpio_enable_port(PERI_GPIOF);
	LONGS_EQUAL(0x2100, PMU->PER);
}

TEST(gpio, disable_port_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_disable_port(PERI_UART1);
}
TEST(gpio, disable_port_ShouldClearPmuPer) {
	PMU->PER = 0x1200;
	gpio_disable_port(PERI_GPIOB);
	LONGS_EQUAL(0x1000, PMU->PER);
	gpio_disable_port(PERI_GPIOE);
	LONGS_EQUAL(0, PMU->PER);
}

TEST(gpio, set_mode_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_set_mode(PERI_UART1, 0, GPIO_MODE_PUSHPULL);
}
TEST(gpio, set_mode_ShouldCauseAssert_WhenUnsupportedGpioGiven) {
	mock().expectOneCall("assert_override");
	gpio_set_mode(PERI_GPIOA, 16, GPIO_MODE_PUSHPULL);
}
TEST(gpio, set_mode_ShouldSetCR_WhenPushpullModeGiven) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFFC, PCA->CR);
	gpio_set_mode(PERI_GPIOA, 1, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFF0, PCA->CR);
	gpio_set_mode(PERI_GPIOA, 2, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0xFFFFFFC0, PCA->CR);
	gpio_set_mode(PERI_GPIOA, 15, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x3FFFFFC0, PCA->CR);
	gpio_set_mode(PERI_GPIOA, 8, GPIO_MODE_PUSHPULL);
	LONGS_EQUAL(0x3FFCFFC0, PCA->CR);
}
TEST(gpio, set_mode_ShouldSetCR_WhenOpendrainModeGiven) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_OPENDRAIN);
	LONGS_EQUAL(0xFFFFFFFD, PCA->CR);
}
TEST(gpio, set_mode_ShouldSetCR_WhenInputModeGiven) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_INPUT);
	LONGS_EQUAL(0xFFFFFFFE, PCA->CR);
	gpio_set_mode(PERI_GPIOB, 0, GPIO_MODE_INPUT_PULLUP);
	LONGS_EQUAL(0xFFFFFFFE, PCB->CR);
	gpio_set_mode(PERI_GPIOC, 0, GPIO_MODE_INPUT_PULLDOWN);
	LONGS_EQUAL(0xFFFFFFFE, PCC->CR);
}
TEST(gpio, set_mode_ShouldSetCR_WhenAnalogModeGiven) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_ANALOG);
	LONGS_EQUAL(0xFFFFFFFF, PCA->CR);
}
TEST(gpio, set_mode_ShouldSetPullup_WhenInputModeWithPullupGiven) {
	gpio_set_mode(PERI_GPIOA, 0, GPIO_MODE_INPUT_PULLUP);
	LONGS_EQUAL(0x1, PCA->PCR);
}
TEST(gpio, set_mode_ShouldSetPulldown_WhenInputModeWithPulldownGiven) {
	gpio_set_mode(PERI_GPIOA, 15, GPIO_MODE_INPUT_PULLDOWN);
	LONGS_EQUAL(0x80008000, PCA->PCR);
}

TEST(gpio, set_debouncer_ShouldCauseAssert_WhenInvalidPortGiven) {
	mock().expectOneCall("assert_override");
	gpio_set_debouncer(PERI_UART1, 0, 0);
}
TEST(gpio, set_debouncer_ShouldCauseAssert_WhenInvalidPinGiven) {
	mock().expectOneCall("assert_override");
	gpio_set_debouncer(PERI_GPIOA, 16, 0);
}
TEST(gpio, set_debouncer_ShouldSetDER_WhenClockCycleGiven) {
	gpio_set_debouncer(PERI_GPIOA, 0, 4);
	LONGS_EQUAL(1, PCA->DER);
	gpio_set_debouncer(PERI_GPIOA, 1, 4);
	LONGS_EQUAL(3, PCA->DER);
	gpio_set_debouncer(PERI_GPIOA, 15, 4);
	LONGS_EQUAL(0x8003, PCA->DER);
}
TEST(gpio, set_debouncer_ShouldClearDER_WhenZeroClockCycleGiven) {
	gpio_set_debouncer(PERI_GPIOA, 0, 4);
	gpio_set_debouncer(PERI_GPIOA, 1, 4);

	gpio_set_debouncer(PERI_GPIOA, 0, 0);
	LONGS_EQUAL(2, PCA->DER);
	gpio_set_debouncer(PERI_GPIOA, 1, 0);
	LONGS_EQUAL(0, PCA->DER);
}
TEST(gpio, set_debouncer_ShouldSetDPR) {
	gpio_set_debouncer(PERI_GPIOA, 0, 4);
	LONGS_EQUAL(0, PCA->DPR);
	gpio_set_debouncer(PERI_GPIOA, 0, 8);
	LONGS_EQUAL(1, PCA->DPR);
	gpio_set_debouncer(PERI_GPIOA, 0, 16);
	LONGS_EQUAL(2, PCA->DPR);
	gpio_set_debouncer(PERI_GPIOA, 0, 32);
	LONGS_EQUAL(3, PCA->DPR);
	gpio_set_debouncer(PERI_GPIOA, 0, 1U << 16);
	LONGS_EQUAL(14, PCA->DPR);
}
