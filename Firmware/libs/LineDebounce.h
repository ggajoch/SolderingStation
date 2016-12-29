#ifndef LIBS_LINEDEBOUNCE_H_
#define LIBS_LINEDEBOUNCE_H_

namespace debouncer {
enum LineState { UNSTABLE, TRANSITION, STABLE };

template <uint32_t debounceTime>
class LineDebounce {
 public:
    LineDebounce() {
        state = UNSTABLE;
        last_value = false;
        times_stable = 0;
    }

    void tick(bool now) {
        switch (state) {
            case UNSTABLE:
                tickUnstable(now);
                break;
            case TRANSITION:
                this->state = STABLE;
                break;
            case STABLE:
                tickStable(now);
                break;
        }
        last_value = now;
    }
    LineState getState() {
        return state;
    }
    bool getValue() {
        return last_value;
    }

 private:
    LineState state;
    bool last_value;
    uint32_t times_stable;

    void tickUnstable(bool now) {
        if (now == last_value) {
            ++times_stable;
        } else {
            times_stable = 0;
        }
        if (times_stable == debounceTime) {
            this->state = TRANSITION;
        }
    }

    void tickStable(bool now) {
        if (now != last_value) {
            times_stable = 0;
            this->state = UNSTABLE;
        }
    }
};
}
#endif  // LIBS_LINEDEBOUNCE_H_
