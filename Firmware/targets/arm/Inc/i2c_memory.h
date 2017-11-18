#ifndef TARGETS_ARM_INC_I2C_MEMORY_H_
#define TARGETS_ARM_INC_I2C_MEMORY_H_

#include <cstdint>
#include <gsl/span>

void i2cMemoryWriteBlock(uint16_t address, gsl::span<const uint8_t> data);
void i2cMemoryReadBlock(std::uint16_t address, gsl::span<uint8_t> data);

#endif  // TARGETS_ARM_INC_I2C_MEMORY_H_
