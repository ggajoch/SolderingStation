#ifndef TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
#define TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_

#include <stdint.h>
#include <array>
#include "storage/storage.h"

typedef struct{
	uint16_t version;
	core::Settings settings;
	uint8_t crc;
} i2cMemorySettingsLayout;

typedef struct{
	uint8_t marker;
	uint16_t temperature;
	uint8_t crc;
} i2cMemoryStateLayout;

typedef struct {
	i2cMemoryStateLayout state;
	uint16_t index;
} i2cMemoryStateLayoutFound;

#endif  // TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
