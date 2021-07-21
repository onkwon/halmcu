#ifndef HALMCU_ADC_HAL_H
#define HALMCU_ADC_HAL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "halmcu/periph/adc.h"
#include "halmcu/ll/adc.h"

void adc_enable(periph_t adc);
void adc_disable(periph_t adc);

#if defined(__cplusplus)
}
#endif

#endif /* HALMCU_ADC_HAL_H */
