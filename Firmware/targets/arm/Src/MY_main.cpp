#include <stdint.h>

#include "core.h"
#include "HAL.h"
#include "STM32_HAL.h"
#include "com.h"

#include "adc.h"
#include "hd44780.h"

extern "C" {
	extern volatile uint8_t tickTimeElapsed;
}

extern "C" void MY_main() {
	TM_HD44780_Init(16, 2);
	//TM_HD44780_Puts(0, 0, "test");

	core::setup();

	float percent = 0;
	HAL::Tip::setHeating(percent);

	while(1) {
		if (tickTimeElapsed) {
			tickTimeElapsed = 0;
			core::tick();

//			percent += 1;
//			if ( percent > 100 ) {
//				percent = 0;
//			}
//			HAL::Tip::setHeating(percent);

//			core::com::printf("temp: %.2f\n", core::tempSensor::getAveragedTemperature());
		}
	}
}

extern "C" void HAL_CmdCallback(char * buf) {
	HAL::Com::callback(buf);
}
