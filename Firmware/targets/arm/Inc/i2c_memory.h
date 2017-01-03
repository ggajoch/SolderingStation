#ifndef TARGETS_ARM_INC_I2C_MEMORY_H_
#define TARGETS_ARM_INC_I2C_MEMORY_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"
#include "i2c.h"
#include "i2c_memory_struct.h"
#include <experimental/optional>

std::experimental::optional<i2cMemorySettings> i2cMemoryReadSettings();
std::experimental::optional<i2cMemoryState> i2cMemoryReadState();

#endif  // TARGETS_ARM_INC_I2C_MEMORY_H_
