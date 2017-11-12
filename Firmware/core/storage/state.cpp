#include <experimental/optional>
#include <reader.h>
#include "state.h"
#include "HAL.h"
#include "layout.h"
#include "config.h"

uint16_t divCeli(uint16_t a, uint16_t b) {
    uint16_t value = a / b;
    if (a % b > 0) {
        value += 1;
    }
    return value;
}

void i2cGetMemoryBounds(uint16_t& i2cMemoryStartAddress, uint16_t& i2cMemorySize) {
    i2cMemoryStartAddress = divCeli(sizeof(i2cMemorySettingsLayout), 8) * 8;

    switch (core::config::memory_type) {
        case core::config::MemoryType::MEM_24C01:
            i2cMemorySize = 128;
            break;
        case core::config::MemoryType::MEM_24C02:
            i2cMemorySize = 256;
            break;
        case core::config::MemoryType::MEM_24C04:
            i2cMemorySize = 512;
            break;
        case core::config::MemoryType::MEM_24C08:
            i2cMemorySize = 1024;
            break;
        case core::config::MemoryType::MEM_24C16:
            i2cMemorySize = 2048;
            break;
    }
}


uint16_t max_slot() {
    uint16_t i2cMemoryStartAddress, i2cMemorySize;
    i2cGetMemoryBounds(i2cMemoryStartAddress, i2cMemorySize);

    return (i2cMemorySize-i2cMemoryStartAddress)/4;
}

uint16_t slot_to_address(uint16_t slot) {
    uint16_t i2cMemoryStartAddress, i2cMemorySize;
    i2cGetMemoryBounds(i2cMemoryStartAddress, i2cMemorySize);

    return i2cMemoryStartAddress + 4*slot;
}

i2cMemoryStateLayout stateLayout;

bool correct_marker(uint16_t slot) {
    HAL::Memory::get(slot_to_address(slot), gsl::span<uint8_t>(&stateLayout.marker, 1));
    return stateLayout.marker == 0x00;
}

bool correct_slot(uint16_t slot) {
    HAL::Memory::get(slot_to_address(slot)+1, gsl::span<uint8_t>((uint8_t*)(&stateLayout)+1, 3));

    return stateLayout.crc_match();
}

void erase_slot(uint16_t slot) {
    std::array<uint8_t, 1> marker{0xFF};
    HAL::Memory::set(slot_to_address(slot), marker);
}

void save_slot(uint16_t slot, i2cMemoryStateLayout data) {
    HAL::Memory::set(slot_to_address(slot), data.as_span());
}

bool find_state(uint16_t slot, uint16_t& temperature) {
    bool found = false;
    for(uint16_t i = 0; i < max_slot(); ++i) {
        if (correct_marker(i)) {
            if (correct_slot(i)) {
                slot = i;
                temperature = stateLayout.temperature;
                found = true;
                break;
            }
            erase_slot(i);
        }
    }

    for(int i = slot+1; i < max_slot(); ++i) {
        if (correct_marker(i)) {
            erase_slot(i);
        }
    }

    return found;
}

std::experimental::optional<core::PersistentState> getState() {
    uint16_t slot = 0, temperature = 0;
    if (find_state(slot, temperature)) {
        return core::PersistentState{.target = temperature};
    }
    return {};
}

void saveState(core::PersistentState state) {
    uint16_t slot = 0, temperature = 0;
    if (!find_state(slot, temperature)) {
        slot = 0;
    }
    erase_slot(slot);
    i2cMemoryStateLayout layout;
    layout.marker = 0x00;
    layout.temperature = state.target;
    layout.crc = layout.calculate_crc();
    save_slot(slot+1, layout);
}