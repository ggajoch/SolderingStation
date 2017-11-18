#pragma once

#include <cstdint>
#include <gsl/span>

void i2cMemoryWriteBlock(uint16_t address, gsl::span<const uint8_t> data);
void i2cMemoryReadBlock(std::uint16_t address, gsl::span<uint8_t> data);
