#ifndef CORE_CONFIG_H_
#define CORE_CONFIG_H_

#include <chrono>

namespace core {
namespace config {

constexpr static int tempAverages = 100;
constexpr static std::chrono::milliseconds tickDuration{100};

constexpr static std::chrono::seconds timeToSaveDataToMemory{10};

};  // namespace config
};  // namespace core

#endif  // CORE_CONFIG_H_
