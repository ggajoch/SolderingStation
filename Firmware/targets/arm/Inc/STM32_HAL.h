#ifndef TARGETS_ARM_INC_STM32_HAL_H_
#define TARGETS_ARM_INC_STM32_HAL_H_

namespace HAL {

namespace Com {
    extern void (*callback)(char * data);
}  // namespace Com

}  // namespace HAL

#endif  // TARGETS_ARM_INC_STM32_HAL_H_
