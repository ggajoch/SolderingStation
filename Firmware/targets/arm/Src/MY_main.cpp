#include <stdint.h>

#include "HAL.h"
#include "STM32_HAL.h"
#include "com.h"
#include "core.h"

#include "adc.h"
#include "hd44780.h"

#include "encoder_hw.h"

extern "C" {
extern volatile uint8_t tickTimeElapsed;
}

extern "C" void MY_main() {
    TM_HD44780_Init(16, 2);
    encoderInit();

    TM_HD44780_Puts(4, 0, "station");
    HAL::delay(1000);

    core::setup();

    float percent = 0;
    HAL::Tip::setHeating(percent);

    while (1) {
        if (tickTimeElapsed) {
            tickTimeElapsed = 0;
            core::tick();
        }
    }
}

extern "C" void HAL_CmdCallback(char* buf) {
    HAL::Com::callback(buf);
}
