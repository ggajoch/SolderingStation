#include <stdint.h>

#include "adc.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"

#include "hd44780.h"

#include "HAL.h"
#include "core.h"

#include "core_support.h"

volatile bool tickTimeElapsed = false;

void MY_main() {
    HAL_IWDG_Start(&hiwdg);
    HAL_IWDG_Refresh(&hiwdg);

    GPIO_InitTypeDef pin;
    pin.Pin = GPIO_PIN_8;
    pin.Pull = GPIO_PULLUP;
    pin.Speed = GPIO_SPEED_FREQ_HIGH;
    pin.Mode = GPIO_MODE_OUTPUT_PP;

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    HAL_GPIO_Init(GPIOA, &pin);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);

    pin.Mode = GPIO_MODE_AF_PP;
    HAL_GPIO_Init(GPIOA, &pin);

    HAL_TIM_PWM_Start(&htim3, LCD_BACKLIGHT_CHANNEL);
    HAL_TIM_PWM_Start(&htim4, LCD_CONTRAST_CHANNEL);

    HAL_ADC_Start(&hadc1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);

    HAL_TIM_Base_Start_IT(&htim2);

    TM_HD44780_Init(16, 2);

    core::setup();

    while (1) {
        if (tickTimeElapsed) {
            tickTimeElapsed = false;
            HAL_IWDG_Refresh(&hiwdg);
            core::tick();
        }
    }
}

void HAL_CmdCallback(char* buf) {
    HAL::Com::handler(buf);
}
