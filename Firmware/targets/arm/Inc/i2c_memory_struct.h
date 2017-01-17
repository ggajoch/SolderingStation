#ifndef TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
#define TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_

#include <stdint.h>
#include <array>
#include "storage.h"

typedef struct{
	uint16_t version;
	core::storage::Settings settings;
	uint8_t crc;
} i2cMemorySettingsLayout;

typedef struct{
	uint8_t marker;
	uint16_t temperature;
	uint8_t crc;
} i2cMemoryStateLayout;

#endif  // TARGETS_ARM_INC_I2C_MEMORY_STRUCT_H_
