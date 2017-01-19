#include "i2c_memory.h"
#include "iwdg.h"

#include "reader.h"
#include "writer.h"

#include "com.h"
#include "core.h"

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

void i2cGetMemoryBounds(uint16_t& i2cMemoryStartAddress, uint16_t& i2cMemorySize) {
    i2cMemoryStartAddress = divCeli(sizeof(i2cMemorySettingsLayout), 8) * 8;

    switch (MEMORY_TYPE) {
        case MEM_24C01:
            i2cMemorySize = 128;
            break;
        case MEM_24C02:
            i2cMemorySize = 256;
            break;
        case MEM_24C04:
            i2cMemorySize = 512;
            break;
        case MEM_24C08:
            i2cMemorySize = 1024;
            break;
        case MEM_24C16:
            i2cMemorySize = 2048;
            break;
    }
}

std::experimental::optional<i2cMemoryStateLayoutFound> i2cMemoryFindLastState() {
    uint16_t i2cMemoryStartAddress, i2cMemorySize;
    i2cGetMemoryBounds(i2cMemoryStartAddress, i2cMemorySize);

    uint8_t memoryRead[4];
    i2cMemoryStateLayout stateMemory;
    i2cMemoryStateLayoutFound stateMemoryFound;
    bool found = false;

    for (uint16_t i = i2cMemorySize - 4; i >= i2cMemoryStartAddress; i -= 4) {
        i2cMemoryReadBlock(i, 1, memoryRead);
        stateMemory.marker = memoryRead[0];

        if (stateMemory.marker == 0x00) {
            if (found) {
                // erase blocks, which should be empty
                memoryRead[0] = 0xFF;
                i2cMemoryWriteBlock(i, 1, memoryRead);
            } else {
                i2cMemoryReadBlock(i + 1, 3, &memoryRead[1]);
                Reader memoryReader(memoryRead);
                stateMemory.marker = memoryReader.ReadByte();
                stateMemory.temperature = memoryReader.ReadWordLE();
                stateMemory.crc = memoryReader.ReadByte();
                if (stateMemory.crc == 0x00) {  // TODO: calculate CRC
                    found = true;
                    stateMemoryFound.state = stateMemory;
                    stateMemoryFound.index = i;
                } else {
                    // erase blocks, with wrong crc
                    memoryRead[0] = 0xFF;
                    i2cMemoryWriteBlock(i, 1, memoryRead);
                }
            }
        }
    }

    if (found) {
        return stateMemoryFound;
    } else {
        return {};
    }
}

std::experimental::optional<core::storage::State> i2cMemoryReadState() {
    auto lastState = i2cMemoryFindLastState();
    if (lastState) {
        core::storage::State state;
        i2cMemoryStateLayoutFound stateFound = *lastState;
        state.targetTemperature = stateFound.state.temperature;
        return state;
    } else {
        return {};
    }
}

void i2cMemoryWriteState(const core::storage::State& state) {
    uint16_t i2cMemoryStartAddress, i2cMemorySize;
    i2cGetMemoryBounds(i2cMemoryStartAddress, i2cMemorySize);

    auto lastState = i2cMemoryFindLastState();

    if (lastState) {
        i2cMemoryStateLayoutFound stateFound = *lastState;
        uint16_t nextIndex = stateFound.index + 4;
        if (nextIndex >= i2cMemorySize) {
            nextIndex = i2cMemoryStartAddress;
        }

        stateFound.state.marker = 0x00;
        stateFound.state.temperature = state.targetTemperature;
        stateFound.state.crc = 0x00;  // TODO: calculate CRC

        uint8_t memoryWrite[4];
        Writer memoryWriter(memoryWrite);
        memoryWriter.WriteByte(stateFound.state.marker);
        memoryWriter.WriteWordLE(stateFound.state.temperature);
        memoryWriter.WriteByte(stateFound.state.crc);

        i2cMemoryWriteBlock(nextIndex, 4, memoryWrite);

        memoryWrite[0] = 0xFF;

        i2cMemoryWriteBlock(stateFound.index, 1, memoryWrite);

        core::com::printf("@%d\n", nextIndex);

    } else {
        i2cMemoryStateLayout stateMemory;
        stateMemory.marker = 0x00;
        stateMemory.temperature = state.targetTemperature;
        stateMemory.crc = 0x00;  // TODO: calculate crc

        uint8_t memoryWrite[4];
        Writer memoryWriter(memoryWrite);
        memoryWriter.WriteByte(stateMemory.marker);
        memoryWriter.WriteWordLE(stateMemory.temperature);
        memoryWriter.WriteByte(stateMemory.crc);

        i2cMemoryWriteBlock(i2cMemoryStartAddress, 4, memoryWrite);
    }
}
