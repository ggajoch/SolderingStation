#include <queue>
#include <cstdio>
#include <cstring>

#include "HAL.h"
#include "core.h"
#include "com.h"
#include "storage.h"

#include "usart.h"
#include "tim.h"
#include "adc.h"

#include "hd44780.h"

#include "encoder_hw.h"

namespace HAL {

void delay(uint32_t ms) {
    HAL_Delay(ms);
}

namespace Display {
    void setBacklight(float percent) {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, static_cast<uint16_t>(percent));
        core::com::printf("Setting backlight: %f\n", percent);
    }

    void setContrast(float percent) {
        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, percent);
        core::com::printf("Setting contrast: %f\n", percent);
    }

    void write(char array[2][16]) {
        char line1[17], line2[17];
        std::memcpy(line1, array[0], 16);
        line1[16] = '\0';
        std::memcpy(line2, array[1], 16);
        line2[16] = '\0';
        core::com::printf("DISP |%s%s|\n", line1, line2);

        TM_HD44780_Puts(0, 0, line1);
        TM_HD44780_Puts(0, 1, line2);
    }
};  // namespace Display

namespace Tip {
    void setHeating(float percent) {
        if (percent < 0.1) {
//            HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
            GPIO_InitTypeDef pin;
            pin.Pin = GPIO_PIN_8;
            pin.Pull = GPIO_PULLUP;
            pin.Speed = GPIO_SPEED_LOW;
            pin.Mode = GPIO_MODE_OUTPUT_PP;

//            = {
//                    .mode =
//            };
            HAL_GPIO_Init(GPIOA, &pin);
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

        } else {
            MX_TIM1_Init();
            HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

            uint32_t PWMvalue = 2000-20.0*percent;
            __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWMvalue);
        }

//        core::com::printf("HEATING: %f\n", percent);
    }

    uint16_t readRaw() {
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 1000000);
        auto ADCValue = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        return ADCValue;
    }

    bool inStand() {
        return false;
    }
};  // namespace Tip

namespace Com {
    void puts(const char * data) {
        HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t*>(const_cast<char *>(data)), strlen(data), 1000);
    }

    void (*callback)(char * data);
    void setCallback(void (*callback_)(char * data)) {
        callback = callback_;
    }
};  // namespace Com

namespace Encoder {
    int getCountAndReset() {
    	return encoderGetAndReset();
    }
    void setButtonCallback(void (*callback)()) {
    	encoderSetCallback(callback);
    }
};  // namespace Encoder

namespace Memory {
    void store(libs::array_view<const uint8_t> data) {
        Com::puts("SAVING TO MEMORY\n");
    }
    void get(libs::array_view<uint8_t> data) {
        static constexpr core::storage::Elements elements = {
                .targetTemperature = 0,
                .pidParams = {
                        .Kp = 1.0,
                        .Ki = 4.0,
                        .Kd = 0
                },
                .tipParams = {
                        .offset = 20,
                        .gain = 0.11
                },
                .contrast = 27.5,
                .backlight = 100
        };

        std::memcpy(data.data(), &elements, sizeof(core::storage::Elements));
    }
};  // namespace Memory

};  // namespace HAL
