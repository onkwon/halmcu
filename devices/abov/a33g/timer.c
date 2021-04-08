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

static int get_irq_bitpos_from_channel(timer_channel_t channel)
{
	if (channel == TIMER_CHANNEL_1) {
		return 8;
	} else if (channel == TIMER_CHANNEL_2) {
		return 9;
	}
	return -1;
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
				10, 1U << 10, 1);
	}
}

void timer_disable_irq(peripheral_t peri, timer_intr_t events)
{
	if (events & TIMER_IRQ_OVERFLOW) {
		bitop_clear(&get_timer_from_peripheral(peri)->CON, 10);
	}
}

void timer_clear_event(peripheral_t peri, timer_intr_t events)
{
	if (events & TIMER_IRQ_OVERFLOW) {
		bitop_set(&get_timer_from_peripheral(peri)->CON, 14);
	}
}

void timer_enable_channel_irq(peripheral_t peri,
		timer_channel_t channel, timer_intr_t events)
{
	if (events & TIMER_IRQ_COMPARE) {
		int pos = get_irq_bitpos_from_channel(channel);
		if (pos < 0) {
			return;
		}
		bitop_clean_set_with_mask(&get_timer_from_peripheral(peri)->CON,
				(uint32_t)pos, 1U << pos, 1);
	}
}

void timer_disable_channel_irq(peripheral_t peri,
		timer_channel_t channel, timer_intr_t events)
{
	if (events & TIMER_IRQ_COMPARE) {
		int pos = get_irq_bitpos_from_channel(channel);
		if (pos < 0) {
			return;
		}
		bitop_clear(&get_timer_from_peripheral(peri)->CON, (uint32_t)pos);
	}
}

void timer_clear_channel_event(peripheral_t peri,
		timer_channel_t channel, timer_intr_t events)
{
	int pos = get_irq_bitpos_from_channel(channel);
	if (pos < 0 || !(events & TIMER_IRQ_COMPARE)) {
		return;
	}

	bitop_set(&get_timer_from_peripheral(peri)->CON, (uint32_t)pos + 4);
}

void timer_start(peripheral_t peri)
{
	bitop_set(&get_timer_from_peripheral(peri)->CMD, 0);
}

void timer_stop(peripheral_t peri)
{
	bitop_clear(&get_timer_from_peripheral(peri)->CMD, 0);
}
