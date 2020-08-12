#pragma once

#include <cmath>
#include <algorithm>
#include "settings.h"

namespace core {

// PID with clamping anti-windup method

class PID {
 public:
    float target;
    float lowerLimit, upperLimit;
    float error;
    float prevError;
    float integral;

    constexpr PID() noexcept : target{0}, lowerLimit{0}, upperLimit{100}, error{0}, prevError{0}, integral{0} {
        this->reset();
    }

    float tick(float temp) {
        float actualUpperLimit = std::min(upperLimit, static_cast<float>(settings.pid.max_power));

        error = target + 0.5f - temp;

        float diff = error - prevError;
        float pwr = settings.pid.Kp * error + settings.pid.Ki * (integral + error) + settings.pid.Kd * diff;

        if (lowerLimit < pwr && pwr < actualUpperLimit) {
            integral += error;
        }

        prevError = error;

        return std::clamp(pwr, lowerLimit, actualUpperLimit);
    }

    void reset() {
        this->integral = 0;
        this->prevError = 0;
    }
};

}  // namespace core
