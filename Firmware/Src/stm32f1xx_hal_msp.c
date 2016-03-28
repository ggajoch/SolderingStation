#include <stdint.h>
#include <stm32f103xb.h>
#include "stm32f1xx_hal.h"

void HAL_MspInit(void) {
    __HAL_RCC_AFIO_CLK_ENABLE();
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
    GPIO_InitTypeDef GPIO_InitStruct;
    if (hadc->Instance == ADC1) {
        __ADC1_CLK_ENABLE();
        /**ADC1 GPIO Configuration
         PA0-WKUP     ------> ADC1_IN0
         */
        GPIO_InitStruct.Pin = ThemocoupleAIN_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        HAL_GPIO_Init(ThemocoupleAIN_GPIO_Port, &GPIO_InitStruct);
    }

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc) {
    if (hadc->Instance == ADC1) {
        __ADC1_CLK_DISABLE();
        /**ADC1 GPIO Configuration
         PA0-WKUP     ------> ADC1_IN0
         */
        HAL_GPIO_DeInit(ThemocoupleAIN_GPIO_Port, ThemocoupleAIN_Pin);
    }
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c) {
    GPIO_InitTypeDef GPIO_InitStruct;
    if (hi2c->Instance == I2C1) {
        /**I2C1 GPIO Configuration
         PB6     ------> I2C1_SCL
         PB7     ------> I2C1_SDA
         */
        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        __I2C1_CLK_ENABLE();
    }

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
        __I2C1_CLK_DISABLE();
        /**I2C1 GPIO Configuration
         PB6     ------> I2C1_SCL
         PB7     ------> I2C1_SDA
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base) {
    GPIO_InitTypeDef GPIO_InitStruct;
    if (htim_base->Instance == TIM1) {
        __TIM1_CLK_ENABLE();
        /**TIM1 GPIO Configuration
         PA8     ------> TIM1_CH1
         */
        GPIO_InitStruct.Pin = PWM_MOS_OUT_Pin;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
        HAL_GPIO_Init(PWM_MOS_OUT_GPIO_Port, &GPIO_InitStruct);
    } else if (htim_base->Instance == TIM2) {
        __TIM2_CLK_ENABLE();
    } else if (htim_base->Instance == TIM3) {
        __TIM3_CLK_ENABLE();
        /**TIM3 GPIO Configuration
         PB1     ------> TIM3_CH4
         */
        GPIO_InitStruct.Pin = GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    } else if (htim_base->Instance == TIM4) {
        __TIM4_CLK_ENABLE();
        /**TIM4 GPIO Configuration
         PB9     ------> TIM4_CH4
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base) {
    if (htim_base->Instance == TIM1) {
        __TIM1_CLK_DISABLE();
        /**TIM1 GPIO Configuration
         PA8     ------> TIM1_CH1
         */
        HAL_GPIO_DeInit(PWM_MOS_OUT_GPIO_Port, PWM_MOS_OUT_Pin);
    } else if (htim_base->Instance == TIM3) {
        __TIM3_CLK_DISABLE();
        /**TIM3 GPIO Configuration
         PB1     ------> TIM3_CH4
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
    } else if (htim_base->Instance == TIM4) {
        __TIM4_CLK_DISABLE();
        /**TIM4 GPIO Configuration
         PB9     ------> TIM4_CH4
         */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_9);
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitStruct;
    if (huart->Instance == USART1) {
        __USART1_CLK_ENABLE();
        /**USART1 GPIO Configuration
         PA9     ------> USART1_TX
         PA10     ------> USART1_RX
         */
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        __USART1_CLK_DISABLE();
        /**USART1 GPIO Configuration
         PA9     ------> USART1_TX
         PA10     ------> USART1_RX
         */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    }
}
