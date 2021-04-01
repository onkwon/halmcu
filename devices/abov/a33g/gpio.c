#include "abov/hal/gpio.h"
#include "abov/asm/arm/cmsis.h"
#include "a33g.h"
#include "abov/compiler.h"

/* a33g supports 15 pins per port */
#define MAX_PIN_NUMBER				15U
#define PIN_NUMBER_MASK				(GPIO_PORT_SIZE - 1)
#define PER_BASE_POS				8U

static uint32_t get_pin_from_gpio_number(uint32_t ngpio)
{
	return ngpio & PIN_NUMBER_MASK;
}

static gpio_port_t get_port_from_gpio_number(uint32_t ngpio)
{
	return ngpio & ~PIN_NUMBER_MASK;
}

static PCU_Type *get_pcu_from_gpio_number(uint32_t ngpio)
{
	switch (ngpio & ~PIN_NUMBER_MASK) {
	case GPIOA:
		return PCA;
	case GPIOB:
		return PCB;
	case GPIOC:
		return PCC;
	case GPIOD:
		return PCD;
	case GPIOE:
		return PCE;
	case GPIOF:
		return PCF;
	default:
		return NULL;
	}
}

static GPIO_Type *get_reg_from_gpio_number(uint32_t ngpio)
{
	switch (ngpio & ~PIN_NUMBER_MASK) {
	case GPIOA:
		return PA;
	case GPIOB:
		return PB;
	case GPIOC:
		return PC;
	case GPIOD:
		return PD;
	case GPIOE:
		return PE;
	case GPIOF:
		return PF;
	default:
		return NULL;
	}
}

static void enable_port(gpio_port_t port)
{
	uint32_t pos = port >> 8;
	PMU->PER |= 1U << (pos + PER_BASE_POS);
}
ABOV_STATIC_ASSERT(GPIO_PORT_SIZE == 0x100, "");

static void disable_port(gpio_port_t port)
{
	uint32_t pos = port >> 8;
	PMU->PER &= ~(1U << (pos + PER_BASE_POS));
}

static void set_gpio_mode(PCU_Type *ctrl, uint32_t pin, gpio_mode_t mode)
{
	uint32_t pos = pin * 2;
	uint32_t mask = 3U << pos;
	uint32_t reg = ctrl->CR;
	uint32_t val = 0;

	switch (mode) {
	case GPIO_MODE_OPENDRAIN:
		val = 1;
		break;
	case GPIO_MODE_INPUT:
	case GPIO_MODE_INPUT_PULLUP:
	case GPIO_MODE_INPUT_PULLDOWN:
		val = 2;
		break;
	case GPIO_MODE_ANALOG:
		val = 3;
		break;
	case GPIO_MODE_PUSHPULL:
	default:
		break;
	}

	reg &= ~mask;
	reg |= val << pos;
	ctrl->CR = reg;
}

static void set_gpio_pullmode(PCU_Type *ctrl, uint32_t pin, gpio_mode_t mode)
{
	uint32_t reg = ctrl->PCR;
	reg &= ~(1U << pin);
	reg &= ~(1U << (pin + 16));

	if (mode == GPIO_MODE_INPUT_PULLDOWN) {
		reg |= 1U << (pin + 16);
		reg |= 1U << pin;
	} else if (mode == GPIO_MODE_INPUT_PULLUP) {
		reg |= 1U << pin;
	}

	ctrl->PCR = reg;
}

static void set_gpio_alt(PCU_Type *ctrl, uint32_t pin, int altfunc)
{
	uint32_t reg = ctrl->MR;
	uint32_t pos = pin * 2;

	reg &= ~(3U << pos);
	reg |= (uint32_t)altfunc << pos;
	ctrl->MR = reg;
}

static void disable_gpio_intr(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->IER &= ~(3U << (pin * 2));
}

static void disable_gpio_debounce(PCU_Type *ctrl, uint32_t pin)
{
	ctrl->DER &= ~(3U << pin);
}

bool gpio_set_altfunc(uint32_t ngpio, int altfunc)
{
	if (altfunc > 3) {
		return false;
	}

	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	if (ctrl == NULL) {
		return false;
	}

	uint32_t pin = get_pin_from_gpio_number(ngpio);
	if (pin > MAX_PIN_NUMBER) {
		return false;
	}

	set_gpio_alt(ctrl, pin, altfunc);

	return true;
}

static bool set_gpio(uint32_t ngpio, gpio_mode_t mode)
{
	PCU_Type *ctrl = get_pcu_from_gpio_number(ngpio);
	if (ctrl == NULL) {
		return false;
	}

	uint32_t pin = get_pin_from_gpio_number(ngpio);
	if (pin > MAX_PIN_NUMBER) {
		return false;
	}

	enable_port(get_port_from_gpio_number(ngpio));

	set_gpio_mode(ctrl, pin, mode);
	set_gpio_pullmode(ctrl, pin, mode);
	set_gpio_alt(ctrl, pin, 0);
	disable_gpio_intr(ctrl, pin);
	disable_gpio_debounce(ctrl, pin);

	return true;
}

bool gpio_open(uint32_t ngpio, gpio_mode_t mode)
{
	return set_gpio(ngpio, mode);
}

bool gpio_close(uint32_t ngpio)
{
	return set_gpio(ngpio, GPIO_MODE_ANALOG);
}
