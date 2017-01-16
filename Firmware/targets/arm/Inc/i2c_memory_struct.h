#ifndef TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
#define TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_

#include <stdint.h>
#include "pid.h"
#include "tempSensor.h"


typedef struct{
	core::PID::Params pidParams;
	core::tempSensor::Params tipParams;
	float contrast;
	float backlight;
} i2cMemorySettings;

typedef struct{
	uint16_t targetTemperature;
} i2cMemoryState;

typedef struct{
	uint8_t data[sizeof(i2cMemorySettings)];
	uint8_t crc;
}i2cMemorySettingsLayout;

typedef struct{
	uint8_t data[sizeof(i2cMemoryState)];
	uint8_t crc;
}i2cMemoryStateLayout;

#endif  // TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
