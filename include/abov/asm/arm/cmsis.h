#ifndef ABOV_CMSIS_H
#define ABOV_CMSIS_H

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(armv7m0plus)
#include "ARMCM0plus.h"
#elif defined(armv7m3)
#include "ARMCM3.h"
#elif defined(armv7m4)
#include "ARMCM4.h"
#else
#error "not supported cpu architecture"
#endif

#if defined(__cplusplus)
}
#endif

#endif /* ABOV_CMSIS_H */
