/**
  ******************************************************************************
 * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

//#define FLOATING_POINT
#include <stdio.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#include "GPIO/GPIO.h"

#include "UART/uart.h"
#include "ADC/ADC.h"

#include "utils/FIR.h"

#include "PWM.h"
#include "CLI.h"
#include "PC.h"

#include "weller/weller.h"
ADC adc;

extern "C" {
	#include "Delay/delay_ms.h"
	void SysTick_Handler() {
		DelayDecrement();
	}

	void TIM3_IRQHandler() {
//		printf("tick!\r\n");
	}
}

GPIO_t LED(GPIOC, GPIO_Pin_12);

static char buf[100];







int main(void) {
	PC_Init();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	SysTick_Config(SystemCoreClock/10000);
	USART3_Dev.init(115200);
	adc.init();
	printf("ADC Init!\r\n");
	float x = 0;


	TIM3_Dev.init(4800, 0);
	SolderPWM.init(2000, 4800);
	uint16_t val = 0;
	for(;;) {
		SolderPWM.fill(0);
		_delay_ms(10);
		LED.toogle();
		int x = adc.getSample();
		solderingIron.tick(x);

		PC_sendTick(solderingIron.last_temp, solderingIron.filtered_temp,
				    solderingIron.power);


		SolderPWM.fill(solderingIron.power);
		_delay_ms(100);
	}
}
