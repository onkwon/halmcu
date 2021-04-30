#ifndef ABOV_ADC_HAL_H
#define ABOV_ADC_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "abov/ll/adc.h"

void adc_enable(periph_t adc);
void adc_disable(periph_t adc);

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_ADC_HAL_H */
