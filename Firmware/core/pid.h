#ifndef CORE_PID_H_
#define CORE_PID_H_

#include <math.h>

namespace core {

// PID with clamping anti-windup method

class PID {
 public:
    float target;
    float Kp, Ki, Kd;
    float lowerLimit, upperLimit;

    PID() {
        Kp = Ki = Kd = 0;
        target = 0;
        this->lowerLimit = 0;
        this->upperLimit = 0;
        this->reset();
    }

    float tick(float temp) {
        error = temp-target;

        if (lowerLimit < prevOutput < upperLimit)
            integral += error;

        float diff = error - prevError;
        float pwr = (Kp * error + Ki * integral + Kd * diff);

        prevError = error;
        prevOutput = pwr;

        return constrain(pwr);
    }

    void reset() {
        this->integral = 0;
        this->prevError = 0;
        this->prevOutput = 0;
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
    float prevOutput;
    float integral;
};

};  // namespace core

#endif  // CORE_PID_H_
