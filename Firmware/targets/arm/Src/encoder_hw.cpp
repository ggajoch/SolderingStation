#include "encoder_hw.h"

#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

#include "LineDebounce.h"

#include "hardwareConfig.h"

void (*encoderCallback)() = NULL;

volatile int encoderCount = 0, buttonCount = 0;

struct State {
    bool p, n;
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

void encoderSetButtonCallback(void (*callback)()) {
    encoderCallback = callback;
}

void encoderCallbackTick() {
    __disable_irq();
    if (buttonCount > 0) {
        buttonCount--;
        if (encoderCallback != NULL) {
            encoderCallback();
        }
    }
    __enable_irq();
}

debouncer::LineDebounce<ENCODER_DEBOUNCE_STABLE> EncoderDebouncedLineP;
debouncer::LineDebounce<ENCODER_DEBOUNCE_STABLE> EncoderDebouncedLineN;
debouncer::LineDebounce<BUTTON_DEBOUNCE_STABLE> ButtonDebouncedLine;

void encoder10kHzTickISR() {
    State now;
    now.p = HAL_GPIO_ReadPin(ENC_P_GPIO_Port, ENC_P_Pin) == GPIO_PIN_SET;
    now.n = HAL_GPIO_ReadPin(ENC_N_GPIO_Port, ENC_N_Pin) == GPIO_PIN_SET;
    EncoderDebouncedLineP.tick(now.p);
    EncoderDebouncedLineN.tick(now.n);

    if (EncoderDebouncedLineP.getState() == debouncer::TRANSITION && EncoderDebouncedLineN.getState() == debouncer::STABLE) {
        if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
            encoderCount++;
        } else {
            encoderCount--;
        }
    } else if (EncoderDebouncedLineP.getState() == debouncer::STABLE && EncoderDebouncedLineN.getState() == debouncer::TRANSITION) {
        if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
            encoderCount--;
        } else {
            encoderCount++;
        }
    }

    bool btn = HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin) == GPIO_PIN_SET;

    ButtonDebouncedLine.tick(btn);

    if (ButtonDebouncedLine.getState() == debouncer::TRANSITION && ButtonDebouncedLine.getValue() == false) {
        buttonCount++;
    }
}
