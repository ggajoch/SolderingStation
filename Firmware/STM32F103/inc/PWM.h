#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

class TIM_t {
public:
	void init(uint32_t period, uint32_t prescaler) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInitTypeDef timerInitStructure;
		timerInitStructure.TIM_Prescaler = prescaler;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = period;
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &timerInitStructure);
		TIM_Cmd(TIM3, ENABLE);


		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		NVIC_Init(&NVIC_InitStruct);
//		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	}
};

static TIM_t TIM3_Dev;

class PWMChannel {
	uint32_t period_len;
public:
	void init(uint32_t pulse, uint32_t period) {
		period_len = period;
		TIM_OCInitTypeDef outputChannelInit = {0,};
		outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
		outputChannelInit.TIM_Pulse = pulse;
		outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
		outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OC3Init(TIM3, &outputChannelInit);
		TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//		GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
	}
	void pulse(uint16_t set) {
//		TIM_OC3bPreloadConfig(TIM3, set);
		TIM_SetCompare3(TIM3, set);
	}
	void fill(float percent) {
		uint32_t val = 0;
		if( percent > 100 ) {
			val = period_len;
		} else if ( percent <= 0.0 ) {
			val = 0;
		} else {
			val = static_cast<uint32_t>(percent * static_cast<float>(period_len/100.0));
		}

		TIM_SetCompare3(TIM3, val);
	}
};

static PWMChannel SolderPWM;
