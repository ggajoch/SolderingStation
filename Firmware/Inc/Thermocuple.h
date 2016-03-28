#ifndef SOLDER_THERMOCUPLE_H
#define SOLDER_THERMOCUPLE_H

class Thermocouple_t {
public:
    struct ThermocupleCoefficients {
        ThermocupleCoefficients() {
            offset = 0;
            gain = 1;
        }
        float offset;
        float gain;
        float calculate(uint16_t reading) {
            return offset + static_cast<float>(reading) * gain;
        }
    };

    uint16_t getRaw() {
        return HAL::getThermocoupleReading();
    }

    float getTemperature() {
        return coefficients.calculate(getRaw());
    }

    void setCoefficients(const ThermocupleCoefficients & coefficients) {
        this->coefficients = coefficients;
    }

private:
    ThermocupleCoefficients coefficients;
};
#endif //SOLDER_THERMOCUPLE_H
