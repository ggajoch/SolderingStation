#include "encoder_hw.h"

#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

#define DEBOUNCE_STABLE 5
#define ENCODER_PRESC 4

void (*encoderCallback)() = NULL;

volatile int encoderCount = 0, buttonCount = 0;

struct State {
    bool p, n;
};

class LineDebounce {
 public:
    LineDebounce() {
        state = UNSTABLE;
        last_value = false;
        times_stable = 0;
    }
    enum LineState { UNSTABLE, TRANSITION, STABLE };

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
        if (times_stable == DEBOUNCE_STABLE) {
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

void encoderInit() {
    HAL_TIM_Base_Start_IT(&htim2);
}

int encoderGetCountAndReset() {
    __disable_irq();
    int toReturn = encoderCount / ENCODER_PRESC;
    if (toReturn != 0)
        encoderCount = 0;
    __enable_irq();
    return toReturn;
}

void encoderSetCallback(void (*callback)()) {
    encoderCallback = callback;
}

void encoderCallbackTick(){
	__disable_irq();
	if (buttonCount > 0){
		buttonCount--;
		if (encoderCallback != NULL) {
			encoderCallback();
		}
	}
	__enable_irq();
}
LineDebounce LineP, LineN, LineB;

void encoder10kHzTickISR() {
    State now;
    now.p = HAL_GPIO_ReadPin(ENC_P_GPIO_Port, ENC_P_Pin) == GPIO_PIN_SET;
    now.n = HAL_GPIO_ReadPin(ENC_N_GPIO_Port, ENC_N_Pin) == GPIO_PIN_SET;
    LineP.tick(now.p);
    LineN.tick(now.n);

    if (LineP.getState() == LineDebounce::TRANSITION && LineN.getState() == LineDebounce::STABLE) {
        if (LineP.getValue() == LineN.getValue()) {
            encoderCount++;
        } else {
            encoderCount--;
        }
    } else if (LineP.getState() == LineDebounce::STABLE && LineN.getState() == LineDebounce::TRANSITION) {
        if (LineP.getValue() == LineN.getValue()) {
            encoderCount--;
        } else {
            encoderCount++;
        }
    }

    bool btn = HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin) == GPIO_PIN_SET;

    LineB.tick(btn);

    if (LineB.getState() == LineDebounce::TRANSITION && LineB.getValue() == false) {
    	buttonCount++;
    }
}
