#ifndef CORE_TIP_H_
#define CORE_TIP_H_

namespace core {

class Tip {
 public:
    float offset, gain;

    Tip() {
        offset = 0;
        gain = 1;
    }

    float getTemperature(int raw_reading) {
        return offset + gain*static_cast<float>(raw_reading);
    }
};

};  // namespace core

#endif  // CORE_TIP_H_
