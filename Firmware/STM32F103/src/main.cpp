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

#include "utils/AVG.h"

#include "PWM.h"
#include "CLI.h"
#include "PC.h"

#include "LCD/LCD_trans.h"

#include "weller/weller.h"
#include "Delay/delay_ms.h"

#include "Encoder/Encoder.h"
#include "Menu/screen.h"

ADC adc;
GPIO_t pinx(GPIOC, GPIO_Pin_6, GPIO_Mode_Out_PP);
GPIO_t enc_button(GPIOC, GPIO_Pin_9, GPIO_Mode_IPU);

Main menu;


GPIO_t SystickLED(GPIOB, GPIO_Pin_12);
GPIO_t TIM3LED(GPIOB, GPIO_Pin_13);
GPIO_t TIM4LED(GPIOB, GPIO_Pin_14);

extern "C" {

	void SysTick_Handler() {
		SystickLED.on();
		DelayDecrement();

		SystickLED.off();
	}

	void TIM3_IRQHandler() {
		TIM3LED.on();
//		printf("tick!\r\n");
		TIM3LED.off();
	}
	void TIM4_IRQHandler() {
	//		printf("tick!\r\n");
		TIM4LED.on();
		if( TIM_GetITStatus(TIM4, TIM_IT_Update) ) {
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			pinx.toogle();
			LCD_4us_timeproc();
			EncoderProc();

			static bool waitForLow = true;
			static uint32_t wait = 0;

			if( waitForLow ) {
				if( enc_button.read() == 0 ) {
					waitForLow = false;
					wait = 0;
				}
			} else {
				if( wait <  3000 ) {
					wait++;
				} else if( wait == 3000 ) {
					menu.long_click();
					wait++;
				}
				if( enc_button.read() == 1 ) {
					waitForLow = true;
					if( wait < 3000 ) {
						menu.click();
					}
				}
			}
		}
		TIM4LED.off();
	}
}

GPIO_t LED(GPIOC, GPIO_Pin_12);

static char buf[100];



void Plus() {
//	USART3_Dev.puts("+");
	menu.plus();
}

void Minus() {
//	USART3_Dev.puts("-");
	menu.minus();
}





int main(void) {
	GPIO_t LCDContrastPin(GPIOB, GPIO_Pin_8, GPIO_Mode_AF_PP);
	GPIO_t LCDBacklightPin(GPIOB, GPIO_Pin_9, GPIO_Mode_AF_PP);

	EncoderInit();
	EncoderCallbackPlus[0]=Plus;
	EncoderCallbackMinus[0]=Minus;

	// P = 10, I = 0.03, D = 300
	SysTick_Config(SystemCoreClock/10000);
	TIM4_Dev.init(100, 100);
	ContrastPWM.init(100);
	LCDBacklightPWM.init(100);

	ContrastPWM.fill(18);
	LCDBacklightPWM.fill(0);


	LCD_init();
	int i = 0;
	static char buf[16];
	sprintf(buf, "test1: %d", i++);
	LCD_PutText(0, 0, buf);

	PC_Init();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);


	USART3_Dev.init(115200);
	adc.init();
	printf("ADC Init!\r\n");
	float x = 0;



	TIM3_Dev.init(2000, 0);
	SolderPWM.init(2000);

	AVG<float, 20> avg;





	GPIO_t tester(GPIOC, GPIO_Pin_10, GPIO_Mode_IN_FLOATING);
	bool onStand = false;
	for(;;) {
		tester = GPIO_t(GPIOC, GPIO_Pin_10, GPIO_Mode_IN_FLOATING);
		menu.display();
		if( onStand ) {
			if( solderingIron.state == Weller::heat ) {
				solderingIron.state = Weller::onStand;
			}
		} else {
			if( solderingIron.state == Weller::onStand ) {
				solderingIron.state = Weller::heat;
			}
		}

		float setpoint = solderingIron.pid.getTarget();
		float val = 100/(setpoint - 50) * solderingIron.filtered_temp + 5000/(50-setpoint);
		LCDBacklightPWM.fill(val);

		SolderPWM.fill(100);
		_delay_ms(15);
		LED.toogle();
		for(uint8_t i = 0; i < 20; ++i) {
			int x = adc.getSample();
			avg.put(x);
		}

		solderingIron.tick(avg.get());


		PC_sendTick(solderingIron.last_temp, solderingIron.filtered_temp,
		solderingIron.power, solderingIron.filtered_power);


		SolderPWM.fill(100.0-solderingIron.filtered_power);
		tester = GPIO_t(GPIOC, GPIO_Pin_10, GPIO_Mode_IPU);
		_delay_ms(45);
		onStand = !tester.read();
	}
}
