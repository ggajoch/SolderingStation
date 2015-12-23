#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

class GPIO_t {
private:
	GPIO_TypeDef * GPIOx;
	uint16_t pins;
	volatile bool state;
public:
	GPIO_t(GPIO_TypeDef * GPIOx, uint16_t pins) : GPIOx(GPIOx), pins(pins) {
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
				             | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
				             | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF
				             | RCC_APB2Periph_GPIOG, ENABLE);
		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
		GPIO_InitStruct.GPIO_Pin = pins;
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOx, &GPIO_InitStruct);

		off();
	}
	void on() {
		state = true;
		GPIO_SetBits(GPIOx, pins);
	}
	void off() {
		state = false;
		GPIO_ResetBits(GPIOx, pins);
	}
	void toogle() {
		if( state )
			off();
		else
			on();
	}
	void write(bool value) {
		if( value )
			on();
		else
			off();
	}
};
