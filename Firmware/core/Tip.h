#ifndef CORE_TIP_H_
#define CORE_TIP_H_

namespace core {

class Tip {
 public:
    Tip() {
        offset = 0;
        gain = 1;
    }

    float getTemperature(int raw_reading) {
        return offset + gain*static_cast<float>(raw_reading);
    }

    void setParameters(float offset_, float gain_) {
        offset = offset_;
        gain = gain_;
    }

 private:
    float offset, gain;
};

};  // namespace core

#endif  // CORE_TIP_H_
