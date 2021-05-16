#include "abov/ll/timer.h"

#include <stddef.h>

#include "abov/bitop.h"
#include "abov/compiler.h"
#include "abov/assert.h"
#include "stm32f1.h"

static TIM_Type *get_instance(periph_t peri)
{
	switch (peri) {
	case PERIPH_TIM1:
		return TIM1;
	case PERIPH_TIM2:
		return TIM2;
	case PERIPH_TIM3:
		return TIM3;
	case PERIPH_TIM4:
		return TIM4;
	case PERIPH_TIM5:
		return TIM5;
	case PERIPH_TIM6:
		return TIM6;
	case PERIPH_TIM7:
		return TIM7;
	case PERIPH_TIM8:
		return TIM8;
	default:
		assert(0);
		return NULL;
	}
}

void timer_ll_reset(periph_t peri)
{
	volatile uint32_t *rstr = &RCC->APB1RSTR;
	uint32_t pos = 0;

	switch (peri) {
	case PERIPH_TIM8:
		rstr = &RCC->APB2RSTR;
		pos = 12;
		break;
	case PERIPH_TIM1:
		rstr = &RCC->APB2RSTR;
		pos = 11;
		break;
	case PERIPH_TIM7:
		pos++; /* fall through */
	case PERIPH_TIM6:
		pos++; /* fall through */
	case PERIPH_TIM5:
		pos++; /* fall through */
	case PERIPH_TIM4:
		pos++; /* fall through */
	case PERIPH_TIM3:
		pos++; /* fall through */
	case PERIPH_TIM2:
		break;
	default:
		assert(0);
	}

	bitop_set(rstr, pos);
	bitop_clear(rstr, pos);
}

void timer_ll_set_mode(periph_t peri, timer_mode_t mode)
{
	TIM_Type *instance = get_instance(peri);

	bitop_clear(&instance->CR1, 3);

	switch (mode) {
	case TIMER_MODE_ONESHOT:
		bitop_set(&instance->CR1, 3/*OPM*/); /* fall through */
	case TIMER_MODE_PWM: /* fall through */
	case TIMER_MODE_CAPTURE: /* fall through */
	case TIMER_MODE_NORMAL: /* edge aligned */
		bitop_clean_set_with_mask(&instance->CR1, 5, 3, 0);
		/* fall through */
	default:
		break;
	}
}

void timer_ll_set_prescaler(periph_t peri, uint32_t div_factor)
{
	assert(div_factor < 0x10000u); /* 16-bit counter */
	get_instance(peri)->PSC = div_factor;
}

void timer_ll_enable_irq(periph_t peri, timer_event_t events)
{
	TIM_Type *instance = get_instance(peri);
	uint32_t bits = instance->DIER;

	if (events & TIMER_EVENT_UPDATE) {
		bits |= 1u << 0; /* UIE */
	}
	if (events & TIMER_EVENT_CC_1) {
		bits |= 1u << 1; /* CC1IE */
	}
	if (events & TIMER_EVENT_CC_2) {
		bits |= 1u << 2; /* CC2IE */
	}
	if (events & TIMER_EVENT_CC_3) {
		bits |= 1u << 3; /* CC3IE */
	}
	if (events & TIMER_EVENT_CC_4) {
		bits |= 1u << 4; /* CC4IE */
	}

	instance->DIER = bits;
}

void timer_ll_disable_irq(periph_t peri, timer_event_t events)
{
	TIM_Type *instance = get_instance(peri);
	uint32_t bits = instance->DIER;

	if (events & TIMER_EVENT_UPDATE) {
		bits &= ~(1u << 0); /* UIE */
	}
	if (events & TIMER_EVENT_CC_1) {
		bits &= ~(1u << 1); /* CC1IE */
	}
	if (events & TIMER_EVENT_CC_2) {
		bits &= ~(1u << 2); /* CC2IE */
	}
	if (events & TIMER_EVENT_CC_3) {
		bits &= ~(1u << 3); /* CC3IE */
	}
	if (events & TIMER_EVENT_CC_4) {
		bits &= ~(1u << 4); /* CC4IE */
	}

	instance->DIER = bits;
}

void timer_ll_set_counter(periph_t peri, uint32_t value)
{
	assert(value < 0x10000u); /* 16-bit counter */
	get_instance(peri)->CNT = value;
}

uint32_t timer_ll_get_counter(periph_t peri)
{
	return get_instance(peri)->CNT;
}

void timer_ll_set_reload(periph_t peri, uint32_t value)
{
	assert(value < 0x10000u); /* 16-bit counter */
	get_instance(peri)->ARR = value;
}

uint32_t timer_ll_get_reload(periph_t peri)
{
	return get_instance(peri)->ARR;
}

void timer_ll_start(periph_t peri)
{
	TIM_Type *instance = get_instance(peri);
	bitop_set(&instance->CR1, 0); /* CEN */
}

void timer_ll_stop(periph_t peri)
{
	TIM_Type *instance = get_instance(peri);
	bitop_clear(&instance->CR1, 0); /* CEN */
}

void timer_ll_set_cc(periph_t peri, timer_cc_t cc, uint32_t value)
{
	TIM_Type *instance = get_instance(peri);
	if (cc == TIMER_CC_1) {
		instance->CCR1 = value;
	} else if (cc == TIMER_CC_2) {
		instance->CCR2 = value;
	} else if (cc == TIMER_CC_3) {
		instance->CCR3 = value;
	} else if (cc == TIMER_CC_4) {
		instance->CCR4 = value;
	}
}

uint32_t timer_ll_get_cc(periph_t peri, timer_cc_t cc)
{
	if (cc == TIMER_CC_1) {
		return get_instance(peri)->CCR1;
	} else if (cc == TIMER_CC_2) {
		return get_instance(peri)->CCR2;
	} else if (cc == TIMER_CC_3) {
		return get_instance(peri)->CCR3;
	} else if (cc == TIMER_CC_4) {
		return get_instance(peri)->CCR4;
	}

	return 0;
}

void timer_ll_clear_event(periph_t peri, timer_event_t events)
{
	uint32_t value = get_instance(peri)->SR;

	if (events & TIMER_EVENT_UPDATE) {
		value &= ~(1U << 0);
	}
	if (events & TIMER_EVENT_CC_1) {
		value &= ~(1U << 1);
	}
	if (events & TIMER_EVENT_CC_2) {
		value &= ~(1U << 2);
	}
	if (events & TIMER_EVENT_CC_3) {
		value &= ~(1U << 3);
	}
	if (events & TIMER_EVENT_CC_4) {
		value &= ~(1U << 4);
	}

	get_instance(peri)->SR = value;
}

timer_event_t timer_ll_get_event(periph_t peri)
{
	uint32_t value = get_instance(peri)->SR;
	timer_event_t events = TIMER_EVENT_NONE;

	if (value & (1U << 0)) {
		events = (timer_event_t)(events | TIMER_EVENT_UPDATE);
	}
	if (value & (1U << 1)) {
		events = (timer_event_t)(events | TIMER_EVENT_CC_1);
	}
	if (value & (1U << 2)) {
		events = (timer_event_t)(events | TIMER_EVENT_CC_2);
	}
	if (value & (1U << 3)) {
		events = (timer_event_t)(events | TIMER_EVENT_CC_3);
	}
	if (value & (1U << 4)) {
		events = (timer_event_t)(events | TIMER_EVENT_CC_4);
	}

	return events;
}

void timer_ll_set_clock_divider(periph_t peri, uint32_t div_factor)
{
	uint32_t val = 0;

	if (div_factor == 2) {
		val = 1;
	} else if (div_factor == 4) {
		val = 2;
	}

	bitop_clean_set_with_mask(&get_instance(peri)->CR1, 8, 3, val);
}

uint32_t timer_ll_get_frequency(periph_t peri, uint32_t tclk)
{
	TIM_Type *instance = get_instance(peri);
	bool prescaled;

	if (((uint32_t)instance & APB2_BASE) == APB2_BASE) {
		prescaled = (RCC->CFGR >> 13) & 1;
	} else { /* APB1 */
		prescaled = (RCC->CFGR >> 10) & 1;
	}

	if (prescaled) {
		return tclk / 2;
	}

	return tclk;
}

void timer_ll_set_counter_direction(periph_t peri, timer_direction_t dir)
{
	bitop_clean_set_with_mask(&get_instance(peri)->CR1, 4, 1, dir);
}

void timer_ll_set_counter_alignment_mode(periph_t peri, uint32_t align)
{
	bitop_clean_set_with_mask(&get_instance(peri)->CR1, 5, 3, align);
}

void timer_ll_set_cc_pin_mode(periph_t peri, timer_cc_t cc, timer_cc_mode_t mode)
{
	assert(cc >= TIMER_CC_1 && cc <= TIMER_CC_4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16 + 4; /* OCxM */
	if (cc <= TIMER_CC_2) {
		bitop_clean_set_with_mask(&get_instance(peri)->CCMR1,
				pos, 7, mode);
	} else {
		bitop_clean_set_with_mask(&get_instance(peri)->CCMR2,
				pos, 7, mode);
	}
}

void timer_ll_set_cc_pin(periph_t peri, timer_cc_t cc, uint32_t value)
{
	assert(cc >= TIMER_CC_1 && cc <= TIMER_CC_4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16; /* CCxS */
	if (cc <= TIMER_CC_2) {
		bitop_clean_set_with_mask(&get_instance(peri)->CCMR1,
				pos, 3, value);
	} else {
		bitop_clean_set_with_mask(&get_instance(peri)->CCMR2,
				pos, 3, value);
	}
}

void timer_ll_enable_cc_pin(periph_t peri, timer_cc_t cc)
{
	assert(cc > TIMER_CC_0);

	uint32_t pos;

	if (cc > TIMER_CC_4) {
		pos = (uint32_t)((uint32_t)cc - TIMER_CC_1N) * 4 + 2; /* CCxE */
	} else {
		pos = (uint32_t)((uint32_t)cc-1) * 4; /* CCxE */
	}

	bitop_set(&get_instance(peri)->CCER, pos);
}

void timer_ll_disable_cc_pin(periph_t peri, timer_cc_t cc)
{
	assert(cc > TIMER_CC_0);

	uint32_t pos;

	if (cc > TIMER_CC_4) {
		pos = (uint32_t)((uint32_t)cc - TIMER_CC_1N) * 4 + 2; /* CCxE */
	} else {
		pos = (uint32_t)((uint32_t)cc-1) * 4; /* CCxE */
	}

	bitop_clear(&get_instance(peri)->CCER, pos);
}

void timer_ll_set_cc_pin_polarity(periph_t peri, timer_cc_t cc, bool active_high)
{
	assert(cc > TIMER_CC_0);

	uint32_t pos;

	if (cc > TIMER_CC_4) {
		pos = (uint32_t)((uint32_t)cc - TIMER_CC_1N) * 4 + 3; /* CCxP */
	} else {
		pos = (uint32_t)((uint32_t)cc-1) * 4 + 1; /* CCxP */
	}

	if (active_high) {
		bitop_clear(&get_instance(peri)->CCER, pos);
	} else {
		bitop_set(&get_instance(peri)->CCER, pos);
	}
}

void timer_ll_enable_cc_preload(periph_t peri, timer_cc_t cc)
{
	assert(cc > 0 && cc <= 4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16 + 3; /* OCxPE */
	if (cc <= TIMER_CC_2) {
		bitop_set(&get_instance(peri)->CCMR1, pos);
	} else {
		bitop_set(&get_instance(peri)->CCMR2, pos);
	}
}

void timer_ll_disable_cc_preload(periph_t peri, timer_cc_t cc)
{
	assert(cc > 0 && cc <= 4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16 + 3; /* OCxPE */
	if (cc <= TIMER_CC_2) {
		bitop_clear(&get_instance(peri)->CCMR1, pos);
	} else {
		bitop_clear(&get_instance(peri)->CCMR2, pos);
	}
}

void timer_ll_enable_cc_fastmode(periph_t peri, timer_cc_t cc)
{
	assert(cc > 0 && cc <= 4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16 + 2; /* OCxFE */
	if (cc <= TIMER_CC_2) {
		bitop_set(&get_instance(peri)->CCMR1, pos);
	} else {
		bitop_set(&get_instance(peri)->CCMR2, pos);
	}
}

void timer_ll_disable_cc_fastmode(periph_t peri, timer_cc_t cc)
{
	assert(cc > 0 && cc <= 4);

	uint32_t pos = ((uint32_t)cc - 1) * 8 % 16 + 2; /* OCxFE */
	if (cc <= TIMER_CC_2) {
		bitop_clear(&get_instance(peri)->CCMR1, pos);
	} else {
		bitop_clear(&get_instance(peri)->CCMR2, pos);
	}
}

#if 0
void timer_ll_set_ic_prescaler(periph_t peri, timer_cc_t cc, uint32_t value)
void timer_ll_set_ic_filter(periph_t peri, timer_cc_t cc, uint32_t value)

trigger
dma
encoder
break & deadtime
rcr
#endif
