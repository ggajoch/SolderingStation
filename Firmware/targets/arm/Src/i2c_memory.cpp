#include "i2c_memory.h"
#include "iwdg.h"

std::experimental::optional<core::storage::Settings> i2cMemoryReadSettings() {
    uint8_t memoryRead[sizeof(i2cMemorySettingsLayout)];

    HAL_IWDG_Refresh(&hiwdg);

    for (unsigned int i = 0; i < sizeof(i2cMemorySettingsLayout) / 8 + (sizeof(i2cMemorySettingsLayout) % 8 != 0) ? 1 : 0; i++) {
        HAL_I2C_Mem_Read(&hi2c1,
            MEMORY_BASE_ADDRESS,
            0x00 + i * 8,
            0x01,
            &memoryRead[i * 8],
            sizeof(i2cMemorySettingsLayout) > (i + 1) * 8 ? 8 : sizeof(i2cMemorySettingsLayout) - i * 8,
            MEMORY_TIMEOUT_MS);
        HAL_IWDG_Refresh(&hiwdg);
    }

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
    HAL_IWDG_Refresh(&hiwdg);
    for (unsigned int i = 0; i < sizeof(i2cMemorySettingsLayout) / 8 + (sizeof(i2cMemorySettingsLayout) % 8 != 0) ? 1 : 0; i++) {
        HAL_I2C_Mem_Write(&hi2c1,
            MEMORY_BASE_ADDRESS,
            0x00 + i * 8,
            0x01,
            &memoryToWrite[i * 8],
            sizeof(i2cMemorySettingsLayout) > (i + 1) * 8 ? 8 : sizeof(i2cMemorySettingsLayout) - i * 8,
            MEMORY_TIMEOUT_MS);
        HAL_IWDG_Refresh(&hiwdg);
        HAL_Delay(5);
    }
}

std::experimental::optional<core::storage::State> i2cMemoryReadState() {
    static constexpr core::storage::State state = {.targetTemperature = 0};

    return state;
}

void i2cMemoryWriteState(const core::storage::State& state) {
    UNREFERENCED_PARAMETER(state);
}
