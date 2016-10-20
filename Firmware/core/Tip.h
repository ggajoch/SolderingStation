#ifndef SOLDER_THERMOCUPLE_H
#define SOLDER_THERMOCUPLE_H

class Tip {
public:
    Tip() {
        offset = 0;
        gain = 1;
    }

    static float getTemperature(int raw_reading) {
        return offset + gain*static_cast<float>(raw_reading);
    }

    static void setParameters(float offset_, float gain_) {
        offset = offset_;
        gain = gain_;
    }

private:
    static float offset,
                 gain;
};

#endif //SOLDER_THERMOCUPLE_H
