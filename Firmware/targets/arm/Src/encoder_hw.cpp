#include <stdint.h>

#include "stm32f1xx_hal.h"

#include "hardwareConfig.h"
#include "encoder_hw.h"
#include "LineDebounce.h"
#include "HAL.h"

volatile int encoderCount = 0;

struct State {
    bool p, n;
};

int encoderGetCountAndReset() {
    __disable_irq();
    int toReturn = encoderCount / config::encoder_prescaler;
    if (toReturn != 0)
        encoderCount = 0;
    __enable_irq();
    return toReturn;
}

libs::debouncer::LineDebounce<config::encouder_debounce_stable> EncoderDebouncedLineP;
libs::debouncer::LineDebounce<config::encouder_debounce_stable> EncoderDebouncedLineN;
libs::debouncer::LineDebounce<config::button_debounce_stable> ButtonDebouncedLine;

void encoder10kHzTickISR() {
    State now;
    now.p = HAL_GPIO_ReadPin(ENC_P_GPIO_Port, ENC_P_Pin) == GPIO_PIN_SET;
    now.n = HAL_GPIO_ReadPin(ENC_N_GPIO_Port, ENC_N_Pin) == GPIO_PIN_SET;
    EncoderDebouncedLineP.tick(now.p);
    EncoderDebouncedLineN.tick(now.n);

    if (EncoderDebouncedLineP.getState() == libs::debouncer::State::TRANSITION &&
        EncoderDebouncedLineN.getState() == libs::debouncer::State::STABLE) {
        if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
            encoderCount++;
        } else {
            encoderCount--;
        }
    } else if (EncoderDebouncedLineP.getState() == libs::debouncer::State::STABLE &&
        EncoderDebouncedLineN.getState() == libs::debouncer::State::TRANSITION) {
        if (EncoderDebouncedLineP.getValue() == EncoderDebouncedLineN.getValue()) {
            encoderCount--;
        } else {
            encoderCount++;
        }
    }

    bool btn = HAL_GPIO_ReadPin(ENC_B_GPIO_Port, ENC_B_Pin) == GPIO_PIN_SET;

    ButtonDebouncedLine.tick(btn);

    if (ButtonDebouncedLine.getState() == libs::debouncer::State::TRANSITION && ButtonDebouncedLine.getValue() == false) {
        HAL::Encoder::buttonHandler();
    }
}
