#ifndef ABOV_ADC_LL_H
#define ABOV_ADC_LL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/periph/adc.h"

/**
 * @brief Reset ADC
 *
 * This function makes the given ADC the reset default state.
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_ll_reset(periph_t adc);
/**
 * @brief Activate the ADC
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_ll_enable(periph_t adc);
/**
 * @brief Deactivate the ADC
 *
 * @param[in] adc a peripheral enumerated in @ref periph_t
 */
void adc_ll_disable(periph_t adc);
void adc_ll_set_clock_frequency(periph_t adc, uint32_t hz, uint32_t pclk);
uint32_t adc_ll_get_frequency(periph_t adc, uint32_t pclk);

void adc_ll_set_mode(periph_t adc, adc_mode_t mode);
void adc_ll_start(periph_t adc);
void adc_ll_stop(periph_t adc);
void adc_ll_select_channel(periph_t adc, adc_channel_t channel);
void adc_ll_set_trigger(periph_t adc, adc_trigger_t trigger);
uint32_t adc_ll_get_measurement(periph_t adc);
void adc_ll_enable_irq(periph_t adc);
void adc_ll_disable_irq(periph_t adc);
adc_event_t adc_ll_get_event(periph_t adc);
void adc_ll_clear_event(periph_t adc, adc_event_t events);
bool adc_ll_is_busy(periph_t adc);
bool adc_ll_is_completed(periph_t adc);
void adc_ll_set_sample_time(periph_t adc, adc_channel_t channel, uint32_t cycle);
void adc_ll_calibrate(periph_t adc);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ADC_LL_H */
