#ifndef TARGETS_ARM_INC_I2C_MEMORY_H_
#define TARGETS_ARM_INC_I2C_MEMORY_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "main.h"
#include "i2c.h"
#include "i2c_memory_struct.h"
#include "storage/storage.h"
#include "hardwareConfig.h"
#include <experimental/optional>
#include "storage/persistent_state.h"

constexpr static unsigned char MEMORY_BASE_ADDRESS = 0xA0;
constexpr static unsigned int MEMORY_TIMEOUT_MS = 50;
constexpr static unsigned short MEMORY_VERSION = 0x0001;

std::experimental::optional<core::Settings> i2cMemoryReadSettings();
void i2cMemoryWriteSettings(const core::Settings& settings);
std::experimental::optional<core::PersistentState> i2cMemoryReadState();
void i2cMemoryWriteState(const core::PersistentState& state);

#endif  // TARGETS_ARM_INC_I2C_MEMORY_H_
