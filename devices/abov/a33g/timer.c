#include "abov/ll/timer.h"
#include "abov/bitop.h"

#include "abov/asm/arm/cmsis.h"
#include "a33g.h"
#include "abov/compiler.h"

static TIMER_Type *get_timer_from_peripheral(peripheral_t peri)
{
	uint32_t n = peri & (PERIPHERAL_SPACE_SIZE - 1);
	return (TIMER_Type *)(T0_BASE + (n * 2 << 4));
}

void timer_set_prescaler(peripheral_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->PRS,
			0, 0x3ffU, div_factor);
}

void timer_set_divider(peripheral_t peri, uint32_t div_factor)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			4, 7U << 4, div_factor);
}

void timer_set_counter(peripheral_t peri, uint32_t value)
{
	get_timer_from_peripheral(peri)->CNT = value;
}

uint32_t timer_get_counter(peripheral_t peri)
{
	return get_timer_from_peripheral(peri)->CNT;
}

void timer_set_reload(peripheral_t peri, uint32_t value)
{
	get_timer_from_peripheral(peri)->GRA = value;
}

uint32_t timer_get_reload(peripheral_t peri)
{
	return get_timer_from_peripheral(peri)->GRA;
}

void timer_set_mode(peripheral_t peri, timer_mode_t mode)
{
	bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
			0, 3, mode);
}

void timer_enable_irq(peripheral_t peri, timer_intr_t events)
{
	if (events & TIMER_IRQ_OVERFLOW) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				10, 1U << 10, 1); /* TOVE */
	}
	if (events & TIMER_IRQ_COMPARE_0) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				8, 1U << 8, 1); /* TIE0 */
	}
	if (events & TIMER_IRQ_COMPARE_1) {
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				9, 1U << 9, 1); /* TIE1 */
	}
}

void timer_disable_irq(peripheral_t peri, timer_intr_t events)
{
	if (events & TIMER_IRQ_OVERFLOW) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 10); /* TOVE */
	}
	if (events & TIMER_IRQ_COMPARE_0) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 8); /* TIE0 */
	}
	if (events & TIMER_IRQ_COMPARE_1) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 9); /* TIE1 */
	}
}

void timer_clear_event(peripheral_t peri, timer_intr_t events)
{
	if (events & TIMER_IRQ_OVERFLOW) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 14); /* IOVF */
	}
	if (events & TIMER_IRQ_COMPARE_0) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 12); /* TIF0 */
	}
	if (events & TIMER_IRQ_COMPARE_1) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 13); /* TIF1 */
	}
}

void timer_start(peripheral_t peri)
{
	bitop_set(&get_timer_from_peripheral(peri)->CMD, 0); /* TEN */
}

void timer_stop(peripheral_t peri)
{
	bitop_clear(&get_timer_from_peripheral(peri)->CMD, 0); /* TEN */
}

void timer_set_compare(peripheral_t peri, uint32_t ncompare, uint32_t value)
{
	if (ncompare == 0) {
		get_timer_from_peripheral(peri)->GRA = value;
	}
	if (ncompare == 1) {
		get_timer_from_peripheral(peri)->GRB = value;
	}
}

uint32_t timer_get_event(peripheral_t peri)
{
	uint32_t flags = get_timer_from_peripheral(peri)->CON >> 12;
	uint32_t rc = 0;

	if (flags & 1) {
		rc |= TIMER_IRQ_COMPARE_0;
	}
	if (flags & 2) {
		rc |= TIMER_IRQ_COMPARE_1;
	}
	if (flags & 4) {
		rc |= TIMER_IRQ_OVERFLOW;
	}

	return rc;
}

void timer_reset(peripheral_t peri)
{
	TIMER_Type *tim = get_timer_from_peripheral(peri);
	bitop_set(&tim->CMD, 1); /* TCLR */
	tim->CMD = 0;
	tim->CON = 0;
	tim->CON = 0x700; /* clear interrupt flags */
	tim->GRA = 0;
	tim->GRB = 0;
	tim->PRS = 0;
	tim->CNT = 0;
}
