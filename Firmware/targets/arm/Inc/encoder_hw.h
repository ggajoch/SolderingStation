#ifndef TARGETS_ARM_INC_ENCODER_HW_H_
#define TARGETS_ARM_INC_ENCODER_HW_H_

#ifdef __cplusplus
extern "C" {
#endif

int encoderGetCountAndReset();
void encoder10kHzTickISR();

#ifdef __cplusplus
}
#endif

#endif  // TARGETS_ARM_INC_ENCODER_HW_H_
