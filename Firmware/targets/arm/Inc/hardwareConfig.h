#ifndef TARGETS_ARM_INC_HARDWARECONFIG_H_
#define TARGETS_ARM_INC_HARDWARECONFIG_H_

constexpr static int ENCODER_DEBOUNCE_STABLE = 5;
constexpr static int BUTTON_DEBOUNCE_STABLE = 20;
constexpr static int ENCODER_PRESC = 4;
constexpr enum{MEM_24C16, MEM_24C08, MEM_24C04, MEM_24C02, MEM_24C01} MEMORY_TYPE = MEM_24C16;

#endif  // TARGETS_ARM_INC_HARDWARECONFIG_H_