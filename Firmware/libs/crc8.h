#ifndef LIBS_CRC8_H_
#define LIBS_CRC8_H_

#include <cstdint>
#include <gsl/span>

std::uint8_t crc8(const gsl::span<std::uint8_t> data);

#endif  // LIBS_CRC8_H_
