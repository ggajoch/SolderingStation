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
        last_value = false;
        times_stable = 0;
    }

    void tick(bool now) {
        switch (state) {
            case State::UNSTABLE:
                tickUnstable(now);
                break;
            case State::TRANSITION:
                this->state = State::STABLE;
                break;
            case State::STABLE:
                tickStable(now);
                break;
        }
        last_value = now;
    }
    State getState() {
        return state;
    }
    bool getValue() {
        return last_value;
    }

 private:
    State state;
    bool last_value;
    uint32_t times_stable;

    void tickUnstable(bool now) {
        if (now == last_value) {
            ++times_stable;
        } else {
            times_stable = 0;
        }
        if (times_stable == debounceTime) {
            this->state = State::TRANSITION;
        }
    }

    void tickStable(bool now) {
        if (now != last_value) {
            times_stable = 0;
            this->state = State::UNSTABLE;
        }
    }
};
}
}

#endif  // LIBS_LINEDEBOUNCE_H_
