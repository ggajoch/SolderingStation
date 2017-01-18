#include "i2c_memory.h"
#include "iwdg.h"

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

std::experimental::optional<core::storage::Settings> i2cMemoryReadSettings() {
    uint8_t memoryRead[sizeof(i2cMemorySettingsLayout)];

    HAL_IWDG_Refresh(&hiwdg);

    i2cMemoryReadBlock(0x0000, sizeof(i2cMemorySettingsLayout), memoryRead);

    i2cMemorySettingsLayout* settingsMemory = reinterpret_cast<i2cMemorySettingsLayout*>(memoryRead);

    if (settingsMemory->crc == 0 && settingsMemory->version == MEMORY_VERSION) {  // TODO: caculate CRC
        core::storage::Settings settings = settingsMemory->settings;
        return settings;
    } else {
        return {};
    }
}

void i2cMemoryWriteSettings(const core::storage::Settings& settings) {
    i2cMemorySettingsLayout settingsMemory;

    settingsMemory.version = MEMORY_VERSION;
    settingsMemory.settings = settings;
    settingsMemory.crc = 0;  // TODO: calculate crc of data & version

    uint8_t* memoryToWrite = reinterpret_cast<uint8_t*>(&settingsMemory);
    i2cMemoryWriteBlock(0x0000, sizeof(i2cMemorySettingsLayout), memoryToWrite);
}

std::experimental::optional<core::storage::State> i2cMemoryReadState() {
    static constexpr core::storage::State state = {.targetTemperature = 0};

    return state;
}

void i2cMemoryWriteState(const core::storage::State& state) {
    UNREFERENCED_PARAMETER(state);
}
