#include <stdint.h>

#include "HAL.h"
#include "STM32_HAL.h"
#include "com.h"
#include "core.h"
#include "iwdg.h"

#include "adc.h"
#include "hd44780.h"

#include "encoder_hw.h"

extern "C" {
extern volatile uint8_t tickTimeElapsed;
}

extern "C" void MY_main() {
    TM_HD44780_Init(16, 2);
    encoderInit();

    HAL_IWDG_Refresh(&hiwdg);

    core::setup();

    float percent = 0;
    HAL::Tip::setHeating(percent);

    while (1) {
        if (tickTimeElapsed) {
            tickTimeElapsed = 0;
            HAL_IWDG_Refresh(&hiwdg);
            core::tick();
        }
    }
}

extern "C" void HAL_CmdCallback(char* buf) {
    HAL::Com::callback(buf);
}
