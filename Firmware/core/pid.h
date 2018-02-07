#pragma once

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
        float actualUpperLimit = std::min(upperLimit, settings.pidParams.max_power);

        error = target - temp;

        float diff = error - prevError;
        float pwr = settings.pidParams.Kp * error + settings.pidParams.Ki * (integral + error) + settings.pidParams.Kd * diff;

        if (lowerLimit < pwr && pwr < actualUpperLimit) {
            integral += error;
        }

        prevError = error;

        if (pwr < lowerLimit)
            return lowerLimit;

        if (pwr > actualUpperLimit)
            return actualUpperLimit;

        return pwr;
    }

    void reset() {
        this->integral = 0;
        this->prevError = 0;
    }

    float error;
    float prevError;
    float integral;
};

}  // namespace core
