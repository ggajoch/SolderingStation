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

class TIM4_t {
public:
	void init(uint32_t period, uint32_t prescaler) {
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInitTypeDef timerInitStructure;
		timerInitStructure.TIM_Prescaler = prescaler;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = period;
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &timerInitStructure);
		TIM_Cmd(TIM4, ENABLE);


		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
		NVIC_Init(&NVIC_InitStruct);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	}
};

static TIM4_t TIM4_Dev;



template<int channel>
inline void TIM_OCInit(TIM_TypeDef * TIMx, TIM_OCInitTypeDef * OCx) __attribute__((always_inline));

#define DEFINE_TIM_OCInit(ch)                                                   \
	template<>																	\
	inline void TIM_OCInit<ch>(TIM_TypeDef * TIMx, TIM_OCInitTypeDef * OCx) {   \
		TIM_OC ## ch ## Init(TIMx, OCx);                                        \
	}

DEFINE_TIM_OCInit(1)
DEFINE_TIM_OCInit(2)
DEFINE_TIM_OCInit(3)
DEFINE_TIM_OCInit(4)


template<int channel>
inline void TIM_OCPreloadConfig(TIM_TypeDef * TIMx, uint16_t TIM_OCPreload) __attribute__((always_inline));

#define DEFINE_TIM_OCPreloadConfig(ch)                                                  \
	template<>																	        \
	inline void TIM_OCPreloadConfig<ch>(TIM_TypeDef * TIMx, uint16_t TIM_OCPreload) {   \
		TIM_OC ## ch ## PreloadConfig(TIMx, TIM_OCPreload);                             \
	}

DEFINE_TIM_OCPreloadConfig(1)
DEFINE_TIM_OCPreloadConfig(2)
DEFINE_TIM_OCPreloadConfig(3)
DEFINE_TIM_OCPreloadConfig(4)


template<int channel>
inline void TIM_SetCompare(TIM_TypeDef* TIMx, uint16_t Compare) __attribute__((always_inline));

#define PASS(x) (x)
#define DEFINE_TIM_SetCompare(ch)                                           \
	template<>															    \
	inline void TIM_SetCompare<ch>(TIM_TypeDef* TIMx, uint16_t Compare) {   \
		TIM_SetCompare ## ch (TIMx, Compare);                               \
	}

DEFINE_TIM_SetCompare(1)
DEFINE_TIM_SetCompare(2)
DEFINE_TIM_SetCompare(3)
DEFINE_TIM_SetCompare(4)


template<int channel>
class PWMChannel {
	uint32_t period_len;
	TIM_TypeDef * TIMx;
public:
	PWMChannel(TIM_TypeDef * TIMx) : TIMx(TIMx) {}
	void init(uint32_t period) {
		period_len = period;
		TIM_OCInitTypeDef outputChannelInit = {0,};
		outputChannelInit.TIM_OCMode = TIM_OCMode_PWM1;
		outputChannelInit.TIM_Pulse = 0;
		outputChannelInit.TIM_OutputState = TIM_OutputState_Enable;
		outputChannelInit.TIM_OCPolarity = TIM_OCPolarity_High;

		TIM_OCInit<channel>(TIMx, &outputChannelInit);
		TIM_OCPreloadConfig<channel>(TIMx, TIM_OCPreload_Enable);
	}
	void pulse(uint16_t set) {
		TIM_SetCompare<channel>(TIMx, set);
	}
	void fill(float percent) {
		uint16_t val = 0;
		if( percent >= 99.9 ) {
			val = period_len;
		} else if ( percent <= 0.1 ) {
			val = 0;
		} else {
			val = static_cast<uint16_t>(percent * static_cast<float>(period_len/100.0));
		}
		this->pulse(val);
	}
};

static PWMChannel<3> SolderPWM(TIM3);
static PWMChannel<3> ContrastPWM(TIM4);
static PWMChannel<4> LCDBacklightPWM(TIM4);
