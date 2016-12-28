#ifndef TARGETS_ARM_INC_ENCODER_HW_H_
#define TARGETS_ARM_INC_ENCODER_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"

void encoderInit();
int encoderGetCountAndReset();
void encoderSetCallback(void (*callback)());

void encoder10kHzTickISR();
void encoderCallbackTick();

#ifdef __cplusplus
}
#endif

#endif  // TARGETS_ARM_INC_ENCODER_HW_H_
