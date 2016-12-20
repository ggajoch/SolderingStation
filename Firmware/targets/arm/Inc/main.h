/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define LCD_BACKLIGHT_CHANNEL 12
#define LCD_CONTRAST_CHANNEL 12

#define ENC_B_Pin GPIO_PIN_13
#define ENC_B_GPIO_Port GPIOC
#define ENC_N_Pin GPIO_PIN_14
#define ENC_N_GPIO_Port GPIOC
#define ENC_P_Pin GPIO_PIN_15
#define ENC_P_GPIO_Port GPIOC
#define ADC1_IN0_Pin GPIO_PIN_0
#define ADC1_IN0_GPIO_Port GPIOA
#define LCD_BACKLIGHT_Pin GPIO_PIN_1
#define LCD_BACKLIGHT_GPIO_Port GPIOB
#define LCD_D7_Pin GPIO_PIN_2
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_10
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_11
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_12
#define LCD_D4_GPIO_Port GPIOB
#define LCD_D3_Pin GPIO_PIN_13
#define LCD_D3_GPIO_Port GPIOB
#define LCD_D2_Pin GPIO_PIN_14
#define LCD_D2_GPIO_Port GPIOB
#define LCD_D1_Pin GPIO_PIN_15
#define LCD_D1_GPIO_Port GPIOB
#define LCD_D0_Pin GPIO_PIN_3
#define LCD_D0_GPIO_Port GPIOB
#define LCD_EN_Pin GPIO_PIN_4
#define LCD_EN_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_8
#define LCD_RS_GPIO_Port GPIOB
#define LCD_CONTRAST_Pin GPIO_PIN_9
#define LCD_CONTRAST_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
