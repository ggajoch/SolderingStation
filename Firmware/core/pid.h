#ifndef CORE_PID_H_
#define CORE_PID_H_

#include <cmath>
#include "settings.h"

namespace core {

// PID with clamping anti-windup method

class PID {
 public:
    float target;
    float lowerLimit, upperLimit;

    PID() {
        target = 0;
        this->lowerLimit = 0;
        this->upperLimit = 100;
        this->reset();
    }

    float tick(float temp) {
        error = target - temp;

        float diff = error - prevError;
        float pwr = settings.pidParams.Kp * error +
                    settings.pidParams.Ki * (integral + error) +
                    settings.pidParams.Kd * diff;

        if (lowerLimit < pwr && pwr < upperLimit) {
            integral += error;
        }

        prevError = error;

        return constrain(pwr);
    }

    void reset() {
        this->integral = 0;
        this->prevError = 0;
    }

    float constrain(float val) {
        if (val < lowerLimit)
            return lowerLimit;
        if (val > upperLimit)
            return upperLimit;
        return val;
    }

    float error;
    float prevError;
    float integral;
};

}  // namespace core

#endif  // CORE_PID_H_
