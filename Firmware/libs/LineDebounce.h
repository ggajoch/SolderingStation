#ifndef LIBS_LINEDEBOUNCE_H_
#define LIBS_LINEDEBOUNCE_H_

#include <cstdint>

using std::uint32_t;

namespace libs {
namespace debouncer {
enum class State { UNSTABLE, TRANSITION, STABLE };

template <uint32_t debounceTime>
class LineDebounce {
 public:
    LineDebounce() {
        state = State::UNSTABLE;
        lastValue = false;
        stableValue = false;
        timesStable = 0;
    }

    void tick(bool now) {
        switch (state) {
            case State::UNSTABLE:
                tickUnstable(now);
                break;
            case State::TRANSITION:
                this->state = State::STABLE;
                tickStable(now);
                break;
            case State::STABLE:
                tickStable(now);
                break;
        }
        lastValue = now;
    }
    State getState() {
        return state;
    }
    bool getValue() {
        return stableValue;
    }

 private:
    State state;
    bool lastValue;
    bool stableValue;
    uint32_t timesStable;

    void tickUnstable(bool now) {
        if (now == lastValue) {
            ++timesStable;
        } else {
            timesStable = 0;
        }
        if (timesStable == debounceTime) {
            if (now == stableValue) {
                this->state = State::STABLE;
            } else {
                stableValue = now;
                this->state = State::TRANSITION;
            }
        }
    }

    void tickStable(bool now) {
        if (now != lastValue) {
            timesStable = 0;
            this->state = State::UNSTABLE;
        }
    }
};
}
}

#endif  // LIBS_LINEDEBOUNCE_H_
