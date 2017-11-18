#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <chrono>

namespace core {
namespace config {

constexpr static int tempAverages = 100;
constexpr static std::chrono::milliseconds tickDuration{100};

constexpr static std::chrono::seconds timeToSaveDataToMemory{10};

constexpr static uint8_t memory_version = 0x01;

enum class MemoryType : uint16_t {
    MEM_24C01 = 128,
    MEM_24C02 = 256,
    MEM_24C04 = 512,
    MEM_24C08 = 1024,
    MEM_24C16 = 2048,
};

constexpr static MemoryType memory_type = MemoryType::MEM_24C01;

}  // namespace config
}  // namespace core

#endif  // CORE_CONFIG_H_
