#ifndef ENCODER_H_
#define ENCODER_H_

namespace Encoder {
    constexpr uint8_t DEBOUNCE_STABLE = 5;

    struct State { __attribute__((packed))
        bool p, n;
    };

    class Encoder {
    public:
        Encoder(std::function<State()> getState,
                          std::function<void()> callbackPlus, std::function<void()> callbackMinus) :
                getState(getState), callbackPlus(callbackPlus), callbackMinus(callbackMinus) {

        }

        void Tick() {
            State now = getState();

            LineA.tick(now.p);
            LineB.tick(now.n);

            if( LineA.getState() == LineDebounce::TRANSITION &&
                LineB.getState() == LineDebounce::STABLE ) {
                if( LineA.getValue() == LineB.getValue() ) {
                    callbackPlus();
                } else {
                    callbackMinus();
                }
            } else if( LineA.getState() == LineDebounce::STABLE &&
                LineB.getState() == LineDebounce::TRANSITION ) {
                if( LineA.getValue() == LineB.getValue() ) {
                    callbackMinus();
                } else {
                    callbackPlus();
                }
            }
        }

    private:
        class LineDebounce {
        public:
            LineDebounce() {
                state = UNSTABLE;
                last_value = false;
                times_stable = 0;
            }
            enum LineState {
                UNSTABLE,
                TRANSITION,
                STABLE
            };

            void tick(bool now) {
                switch(state) {
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
            inline LineState getState() {
                return state;
            }
            inline bool getValue() {
                return last_value;
            }
        private:
            LineState state;
            bool last_value;
            uint8_t times_stable;


            void tickUnstable(bool now) {
                if( now == last_value ) {
                    ++times_stable;
                } else {
                    times_stable = 0;
                }
                if( times_stable == DEBOUNCE_STABLE ) {
                    this->state = TRANSITION;
                }
            }

            void tickStable(bool now) {
                if( now != last_value ) {
                    times_stable = 0;
                    this->state = UNSTABLE;
                }
            }
        };
        const std::function<State()> getState;
        const std::function<void()> callbackPlus, callbackMinus;
        LineDebounce LineA, LineB;
    };
}
#endif //ENCODER_H_
