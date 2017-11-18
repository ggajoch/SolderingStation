#pragma once

#if __cplusplus
namespace config {
constexpr static int encouder_debounce_stable = 5;
constexpr static int button_debounce_stable = 20;
constexpr static int encoder_prescaler = 4;

constexpr static unsigned char memory_base_address = 0b1010000;
constexpr static unsigned int memory_timeout_ms = 50;
}  // namespace config
#endif

#define CONFIG_USART1_buffer_size 100
