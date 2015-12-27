#ifndef ENCODER_H_
#define ENCODER_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define ENCODER_DEBOUNCE 5

void (*EncoderCallbackPlus[1])();
void (*EncoderCallbackMinus[1])();


void emptyCall() {

}

void EncoderInit(void){
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	EncoderCallbackPlus[0]=emptyCall;
	EncoderCallbackMinus[0]=emptyCall;
}

void EncoderProc(void){
	static uint8_t last[1][2];
	uint8_t actual[1][2];
	static uint8_t debounce[1][2];
	static uint8_t signalB[1];

	actual[0][0] = (uint8_t)GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);

	if( actual[0][0] == last[0][0] ){
		if( debounce[0][0] < ENCODER_DEBOUNCE ){
			debounce[0][0]++;
		}else if( debounce[0][0] == ENCODER_DEBOUNCE ){
			debounce[0][0]++;
			if(signalB[0]){
				if(actual[0][0] == 0){
					//uart_puts("L\r\n");
					EncoderCallbackPlus[0]();
				}else{
					//uart_puts("R\r\n");
					EncoderCallbackMinus[0]();
				}
			}else{
				if(actual[0][0] == 1){
					//uart_puts("L\r\n");
					EncoderCallbackPlus[0]();
				}else{
					//uart_puts("R\r\n");
					EncoderCallbackMinus[0]();
				}
			}
		}
	}else{
		debounce[0][0] = 0;
		last[0][0] = actual[0][0];
	}

	actual[0][1] = (uint8_t)GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	if( actual[0][1] == last[0][1] ){
		if( debounce[0][1] < ENCODER_DEBOUNCE ){
			debounce[0][1]++;
		}else if( debounce[0][1] == ENCODER_DEBOUNCE ){
			debounce[0][1]++;
			signalB[0] = last[0][1];
		}
	}else{
		debounce[0][1] = 0;
		last[0][1] = actual[0][1];
	}
}



#endif //ENCODER_H_
