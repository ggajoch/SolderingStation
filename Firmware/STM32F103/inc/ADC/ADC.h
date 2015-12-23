#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

class ADC {
public:
	void init() {
		GPIO_InitTypeDef GPIO_InitStructure;
		ADC_InitTypeDef ADC_InitStructure;

		/* Enable ADC1 and GPIOA clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA,
				ENABLE);

		/* Configure PA.01 (ADC Channel1) as analog input -------------------------*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		/* ADC1 configuration ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE; // Single Channel
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // Scan on Demand
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);

		/* ADC1 regular channel1 configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1,
				ADC_SampleTime_55Cycles5);

		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* Enable ADC1 reset calibaration register */
		ADC_ResetCalibration(ADC1);

		/* Check the end of ADC1 reset calibration register */
		while (ADC_GetResetCalibrationStatus(ADC1))
			;

		/* Start ADC1 calibaration */
		ADC_StartCalibration(ADC1);

		/* Check the end of ADC1 calibration */
		while (ADC_GetCalibrationStatus(ADC1))
			;

		/* Start ADC1 Software Conversion */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
	uint16_t getSample() {
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) {
		}
		uint16_t adc = ADC_GetConversionValue(ADC1);
		return adc;
	}
};
