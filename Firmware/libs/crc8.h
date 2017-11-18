#pragma once

#include <cstdint>
#include <gsl/span>

std::uint8_t crc8(const gsl::span<std::uint8_t> data);
