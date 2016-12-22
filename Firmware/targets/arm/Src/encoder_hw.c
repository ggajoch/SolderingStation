#include "encoder_hw.h"

#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h"

void encoderInit(void){
	HAL_TIM_Base_Start_IT(&htim2);
}

void encoderTimCallback(void){

}
