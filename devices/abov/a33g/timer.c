#include "abov/ll/timer.h"
#include "abov/bitop.h"

#include "abov/asm/arm/cmsis.h"
#include "a33g.h"
#include "abov/compiler.h"
#include "abov/assert.h"

ABOV_STATIC_ASSERT(TIMER_MODE_NORMAL == 0, "");
ABOV_STATIC_ASSERT(TIMER_MODE_PWM == 1, "");
ABOV_STATIC_ASSERT(TIMER_MODE_ONESHOT == 2, "");
ABOV_STATIC_ASSERT(TIMER_MODE_CAPTURE == 3, "");

static TIMER_Type *get_instance(periph_t peri)
{
	assert(peri >= PERIPH_TIMER0 && peri <= PERIPH_TIMER9);

	uint32_t timer_index = peri - PERIPH_TIMER0;
	return (TIMER_Type *)(T0_BASE + (timer_index * 2 << 4));
}

void timer_ll_set_prescaler(periph_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_instance(peri)->PRS,
			0, 0x3ffU, div_factor);
}

void timer_ll_set_clock_divider(periph_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_instance(peri)->CON,
			4, 7U, div_factor); /* TCS */
}

void timer_ll_set_counter(periph_t peri, uint32_t value)
{
	get_instance(peri)->CNT = value;
}

uint32_t timer_ll_get_counter(periph_t peri)
{
	return get_instance(peri)->CNT;
}

void timer_ll_set_reload(periph_t peri, uint32_t value)
{
	get_instance(peri)->GRA = value;
}

uint32_t timer_ll_get_reload(periph_t peri)
{
	return get_instance(peri)->GRA;
}

void timer_ll_set_mode(periph_t peri, timer_mode_t mode)
{
	bitop_clean_set_with_mask(&get_instance(peri)->CON, 0, 3, mode);
}

void timer_ll_enable_irq(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_clean_set_with_mask(&get_instance(peri)->CON,
				10, 1U, 1); /* TOVE */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_clean_set_with_mask(&get_instance(peri)->CON,
				8, 1U, 1); /* TIE0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_clean_set_with_mask(&get_instance(peri)->CON,
				9, 1U, 1); /* TIE1 */
	}
}

void timer_ll_disable_irq(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_clear(&get_instance(peri)->CON, 10); /* TOVE */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_clear(&get_instance(peri)->CON, 8); /* TIE0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_clear(&get_instance(peri)->CON, 9); /* TIE1 */
	}
}

void timer_ll_clear_event(periph_t peri, timer_event_t events)
{
	if (events & TIMER_EVENT_OVERFLOW) {
		bitop_set(&get_instance(peri)->CON, 14); /* IOVF */
	}
	if (events & TIMER_EVENT_CC_0) {
		bitop_set(&get_instance(peri)->CON, 12); /* TIF0 */
	}
	if (events & TIMER_EVENT_CC_1) {
		bitop_set(&get_instance(peri)->CON, 13); /* TIF1 */
	}
}

void timer_ll_start(periph_t peri)
{
	bitop_set(&get_instance(peri)->CMD, 1); /* TCLR */
	bitop_set(&get_instance(peri)->CMD, 0); /* TEN */
}

void timer_ll_stop(periph_t peri)
{
	bitop_clear(&get_instance(peri)->CMD, 0); /* TEN */
}

void timer_ll_set_cc(periph_t peri, timer_cc_t cc, uint32_t value)
{
	if (cc == TIMER_CC_0) {
		get_instance(peri)->GRA = value;
	} else if (cc == TIMER_CC_1) {
		get_instance(peri)->GRB = value;
	}
}

uint32_t timer_ll_get_cc(periph_t peri, timer_cc_t cc)
{
	if (cc == TIMER_CC_0) {
		return get_instance(peri)->GRA;
	} else if (cc == TIMER_CC_1) {
		return get_instance(peri)->GRB;
	}
	return 0;
}

timer_event_t timer_ll_get_event(periph_t peri)
{
	uint32_t flags = get_instance(peri)->CON >> 12;
	uint32_t rc = 0;

	if (flags & 1) {
		rc |= TIMER_EVENT_CC_0;
	}
	if (flags & 2) {
		rc |= TIMER_EVENT_CC_1;
	}
	if (flags & 4) {
		rc |= TIMER_EVENT_OVERFLOW;
	}

	return (timer_event_t)rc;
}

void timer_ll_reset(periph_t peri)
{
	TIMER_Type *tim = get_instance(peri);
	bitop_set(&tim->CMD, 1); /* TCLR */
	tim->CMD = 0;
	tim->CON = 0;
	tim->CON = 0x7000; /* clear interrupt flags */
	tim->GRA = 0;
	tim->GRB = 0;
	tim->PRS = 0;
	tim->CNT = 0;
}

uint32_t timer_ll_get_frequency(periph_t peri, uint32_t tclk)
{
	const TIMER_Type *tim = get_instance(peri);
	uint32_t tcs = (tim->CON >> 4) & 0x7;

	switch (tcs) {
	case 0:
		return tclk / 2;
	case 1:
		return tclk / 4;
	case 2:
		return tclk / 16;
	case 3:
		return tclk / 64;
	default:
		break;
	}

	if (tcs >= 6) {
		return 0;
	}

	return tclk;
}

void timer_ll_set_cc_pin_polarity(periph_t peri, timer_cc_t cc, bool active_high)
{
	unused(cc);
	bitop_clean_set_with_mask(&get_instance(peri)->CON,
			7, 1U, active_high); /* TSTRT */
}

void timer_ll_set_cc_pin_mode(periph_t peri, timer_cc_t cc, timer_cc_mode_t mode)
{
	unused(peri);
	unused(cc);
	unused(mode);
}

void timer_ll_set_cc_pin(periph_t peri, timer_cc_t cc, uint32_t value)
{
	unused(peri);
	unused(cc);
	unused(value);
}

void timer_ll_enable_cc_pin(periph_t peri, timer_cc_t cc)
{
	unused(peri);
	unused(cc);
}

void timer_ll_disable_cc_pin(periph_t peri, timer_cc_t cc)
{
	unused(peri);
	unused(cc);
}

#if 0
void timer_ll_set_clock_edge(periph_t peri, timer_edge_t edge)
{
	bitop_clean_set_with_mask(&get_instance(peri)->CON,
			3, 1U, edge); /* CAPM */
}
#endif
