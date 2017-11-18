#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int encoderGetCountAndReset();
void encoder10kHzTickISR();

#ifdef __cplusplus
}
#endif
