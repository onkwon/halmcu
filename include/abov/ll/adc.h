#ifndef ABOV_ADC_LL_H
#define ABOV_ADC_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/periph/periph.h"

typedef enum {
	ADC_MODE_NORMAL			= 0,
	ADC_MODE_IDLE,
} adc_mode_t;

typedef enum {
	ADC_CHANNEL_0			= 0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10,
	ADC_CHANNEL_11,
	ADC_CHANNEL_12,
	ADC_CHANNEL_13,
	ADC_CHANNEL_14,
	ADC_CHANNEL_15,
	ADC_CHANNEL_MAX,
} adc_channel_t;

typedef enum {
	ADC_TRIGGER_MANUAL		= 0,
	ADC_TRIGGER_TIMER0_CC_0,
	ADC_TRIGGER_TIMER1_CC_0,
	ADC_TRIGGER_TIMER2_CC_0,
	ADC_TRIGGER_TIMER3_CC_0,
	ADC_TRIGGER_TIMER4_CC_0,
	ADC_TRIGGER_TIMER5_CC_0,
	ADC_TRIGGER_TIMER6_CC_0,
	ADC_TRIGGER_TIMER7_CC_0,
	ADC_TRIGGER_MAX,
} adc_trigger_t;

typedef enum {
	ADC_EVENT_NONE			= 0,
	ADC_EVENT_BUSY			= (1U << 0),
	ADC_EVENT_COMPLETE		= (1U << 1),
	ADC_EVENT_MASK			= (3U),
} adc_event_t;

/**
 * Reset ADC
 *
 * This function makes the given ADC the reset default state.
 *
 * :param adc: a peripheral enumerated in :c:type:`periph_t`
 */
void adc_reset(periph_t adc);
/**
 * Enable the ADC
 *
 * :param adc: a peripheral enumerated in :c:type:`periph_t`
 */
void adc_enable(periph_t adc);
/**
 * Disable the ADC
 *
 * :param adc: a peripheral enumerated in :c:type:`periph_t`
 */
void adc_disable(periph_t adc);
/***/
void adc_set_mode(periph_t adc, adc_mode_t mode);
/**
 * Start the ADC conversion
 *
 * :param adc: a peripheral enumerated in :c:type:`periph_t`
 */
void adc_start(periph_t adc);
/**
 * Stop the ADC conversion
 *
 * :param adc: a peripheral enumerated in :c:type:`periph_t`
 */
void adc_stop(periph_t adc);
/***/
void adc_select_channel(periph_t adc, adc_channel_t channel);
/***/
void adc_set_trigger(periph_t adc, adc_trigger_t trigger);
/***/
uint32_t adc_read(periph_t adc);
/***/
void adc_enable_irq(periph_t adc);
/***/
void adc_disable_irq(periph_t adc);
/***/
adc_event_t adc_get_event(periph_t adc);
/***/
void adc_clear_event(periph_t adc, adc_event_t events);
/***/
bool adc_is_busy(periph_t adc);
/***/
bool adc_is_completed(periph_t adc);
/***/
void adc_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ADC_LL_H */
