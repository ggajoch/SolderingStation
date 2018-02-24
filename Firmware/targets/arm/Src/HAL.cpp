#include <cstdio>
#include <cstring>
#include <Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f103xb.h>

#include "HAL.h"
#include "com.h"
#include "core.h"
#include "storage/storage.h"

#include "adc.h"
#include "tim.h"
#include "usart.h"

#include "hd44780.h"

#include "encoder_hw.h"

#include "i2c_memory.h"

namespace HAL {

void delay(uint32_t ms) {
    HAL_Delay(ms);
}

namespace Display {
void set(uint8_t backlight_percent, uint8_t contrast_percent) {
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, backlight_percent);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, contrast_percent);
}

void write(char line1[17], char line2[17]) {
    TM_HD44780_Puts(0, 0, line1);
    TM_HD44780_Puts(0, 1, line2);
}
}  // namespace Display

namespace Tip {
void setHeating(float percent) {
    GPIO_InitTypeDef pin;
    pin.Pin = GPIO_PIN_8;
    pin.Pull = GPIO_PULLUP;
    pin.Speed = GPIO_SPEED_FREQ_HIGH;
    pin.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOA, &pin);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    if (percent > 0.01) {
        uint32_t PWMvalue = 20.0f * percent;
        pin.Mode = GPIO_MODE_AF_PP;

        MX_TIM1_Init();
        HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

        __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, PWMvalue);

        HAL_GPIO_Init(GPIOA, &pin);
    }
}

uint16_t readRaw() {
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 1000000);
    auto ADCValue = HAL_ADC_GetValue(&hadc1);
    HAL_ADC_Stop(&hadc1);
    return ADCValue;
}

bool inStand() {
    return HAL_GPIO_ReadPin(DETECTOR_GPIO_Port, DETECTOR_Pin) == GPIO_PIN_RESET;
}
}  // namespace Tip

namespace Com {
void puts(const char* data) {
    HAL_UART_Transmit(&huart1, (uint8_t*)(data), strlen(data), 1000);
}
}  // namespace Com

namespace Encoder {
int getCountAndReset() {
    return encoderGetCountAndReset();
}
}  // namespace Encoder

namespace Memory {
void set(uint16_t address, gsl::span<const std::uint8_t> data) {
    i2cMemoryWriteBlock(address, data);
}
void get(uint16_t address, gsl::span<std::uint8_t> data) {
    i2cMemoryReadBlock(address, data);
}
}  // namespace Memory

}  // namespace HAL
