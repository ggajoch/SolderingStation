#include "i2c_memory.h"
#include "iwdg.h"

#include "com.h"
#include "core.h"

#include "HAL.h"

uint16_t divCeli(uint16_t a, uint16_t b) {
    uint16_t value = a / b;
    if (a % b > 0) {
        value += 1;
    }
    return value;
}

void i2cMemoryWriteBlock(uint16_t address, uint16_t dataSize, uint8_t* data) {
    HAL_IWDG_Refresh(&hiwdg);
    for (unsigned int i = 0; i < divCeli(dataSize, 8); i++) {
        HAL_I2C_Mem_Write(&hi2c1,
            MEMORY_BASE_ADDRESS + ((address / 128) & 0b00001110),
            address % 256,
            0x01,
            &data[i * 8],
            dataSize > (i + 1) * 8 ? 8 : dataSize - i * 8,
            MEMORY_TIMEOUT_MS);
        address += 8;
        HAL_IWDG_Refresh(&hiwdg);
        HAL_Delay(5);
    }
}

void i2cMemoryReadBlock(uint16_t address, uint16_t dataSize, uint8_t* data) {
    HAL_IWDG_Refresh(&hiwdg);
    for (unsigned int i = 0; i < divCeli(dataSize, 8); i++) {
        HAL_I2C_Mem_Read(&hi2c1,
            MEMORY_BASE_ADDRESS + ((address / 128) & 0b00001110),
            address % 256,
            0x01,
            &data[i * 8],
            dataSize > (i + 1) * 8 ? 8 : dataSize - i * 8,
            MEMORY_TIMEOUT_MS);
        address += 8;
        HAL_IWDG_Refresh(&hiwdg);
    }
}
