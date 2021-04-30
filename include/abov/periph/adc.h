#ifndef ABOV_ADC_H
#define ABOV_ADC_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "abov/periph/periph.h"

typedef enum {
	ADC_MODE_SINGLE_CONVERSION			= 0,
	ADC_MODE_SINGLE_CONVERSION_MULTI_CHANNEL,
	ADC_MODE_CONTINUOUS_CONVERSION,
	ADC_MODE_CONTINUOUS_CONVERSION_MULTI_CHANNEL,
	ADC_MODE_IDLE,
} adc_mode_t;

typedef enum {
	ADC_CHANNEL_0					= (1U << 0),
	ADC_CHANNEL_1					= (1U << 1),
	ADC_CHANNEL_2					= (1U << 2),
	ADC_CHANNEL_3					= (1U << 3),
	ADC_CHANNEL_4					= (1U << 4),
	ADC_CHANNEL_5					= (1U << 5),
	ADC_CHANNEL_6					= (1U << 6),
	ADC_CHANNEL_7					= (1U << 7),
	ADC_CHANNEL_8					= (1U << 8),
	ADC_CHANNEL_9					= (1U << 9),
	ADC_CHANNEL_10					= (1U << 10),
	ADC_CHANNEL_11					= (1U << 11),
	ADC_CHANNEL_12					= (1U << 12),
	ADC_CHANNEL_13					= (1U << 13),
	ADC_CHANNEL_14					= (1U << 14),
	ADC_CHANNEL_15					= (1U << 15),
	ADC_CHANNEL_MAX					= (1U << 16),
	ADC_CHANNEL_MASK				= (ADC_CHANNEL_MAX - 1),
} adc_channel_t;

typedef enum {
	ADC_TRIGGER_MANUAL				= 0,
	ADC_TRIGGER_TIMER0_CC0,
	ADC_TRIGGER_TIMER1_CC0,
	ADC_TRIGGER_TIMER2_CC0,
	ADC_TRIGGER_TIMER3_CC0,
	ADC_TRIGGER_TIMER4_CC0,
	ADC_TRIGGER_TIMER5_CC0,
	ADC_TRIGGER_TIMER6_CC0,
	ADC_TRIGGER_TIMER7_CC0,
	ADC_TRIGGER_TIMER1_CC1,
	ADC_TRIGGER_TIMER1_CC2,
	ADC_TRIGGER_TIMER1_CC3,
	ADC_TRIGGER_TIMER2_CC2,
	ADC_TRIGGER_TIMER3_CC1,
	ADC_TRIGGER_TIMER3_TRGO,
	ADC_TRIGGER_EXTI11,
	ADC_TRIGGER_MAX,
} adc_trigger_t;

typedef enum {
	ADC_EVENT_NONE					= 0,
	ADC_EVENT_BUSY					= (1U << 0),
	ADC_EVENT_COMPLETE				= (1U << 1),
	ADC_EVENT_MASK					= (3U),
} adc_event_t;

/**
 * @brief Reset ADC
 *
 * This function makes the given ADC the reset default state.
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_reset(periph_t adc);
/**
 * @brief Activate the ADC
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_activate(periph_t adc);
/**
 * @brief Deactivate the ADC
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_deactivate(periph_t adc);
void adc_set_mode(periph_t adc, adc_mode_t mode);
/**
 * @brief Start the ADC conversion
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_start(periph_t adc);
/**
 * @brief Stop the ADC conversion
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_stop(periph_t adc);
void adc_select_channel(periph_t adc, adc_channel_t channel);
void adc_set_trigger(periph_t adc, adc_trigger_t trigger);
uint32_t adc_get_measurement(periph_t adc);
void adc_enable_irq(periph_t adc);
void adc_disable_irq(periph_t adc);
adc_event_t adc_get_event(periph_t adc);
void adc_clear_event(periph_t adc, adc_event_t events);
bool adc_is_busy(periph_t adc);
bool adc_is_completed(periph_t adc);
void adc_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk);
void adc_set_sample_time(periph_t adc, adc_channel_t channel, uint32_t cycle);
void adc_calibrate(periph_t adc);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ADC_H */
