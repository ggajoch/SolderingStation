#include <experimental/optional>
#include "state.h"
#include "HAL.h"
#include "layout.h"
#include "config.h"

uint16_t div_ceil_uint(uint16_t a, uint16_t b) {
    uint16_t value = a / b;
    if (a % b != 0) {
        value += 1;
    }
    return value;
}

static_assert(sizeof(i2cMemorySettingsLayout) % 8 == 0, "Settings have to be 8-byte padded");
static_assert(sizeof(i2cMemoryStateLayout) == 4, "State needs to be 4-byte length");

constexpr static uint16_t max_slot() {
    uint16_t start_address = sizeof(i2cMemorySettingsLayout);
    uint16_t size = static_cast<uint16_t>(core::config::memory_type);

    return (size-start_address)/sizeof(i2cMemoryStateLayout);
}

uint16_t slot_to_address(uint16_t slot) {
    uint16_t start_address = sizeof(i2cMemorySettingsLayout);

    return start_address + sizeof(i2cMemoryStateLayout)*slot;
}

i2cMemoryStateLayout stateLayout;

bool correct_marker(uint16_t slot) {
    HAL::Memory::get(slot_to_address(slot), gsl::span<uint8_t>(&stateLayout.marker, 1));
    return stateLayout.marker == 0x00;
}

bool get_slot(uint16_t slot) {
    HAL::Memory::get(slot_to_address(slot), stateLayout.as_span());
    return stateLayout.crc_match();
}

void erase_slot(uint16_t slot) {
    std::array<uint8_t, 1> marker{0xFF};
    HAL::Memory::set(slot_to_address(slot), marker);
}

void save_slot(uint16_t slot, i2cMemoryStateLayout data) {
    HAL::Memory::set(slot_to_address(slot), data.as_span());
}

bool find_state(uint16_t& slot, uint16_t& temperature) {
    bool found = false;
    for(uint16_t i = 0; i < max_slot(); ++i) {
        if (correct_marker(i)) {
            if (get_slot(i)) {
                slot = i;
                temperature = stateLayout.temperature;
                found = true;
                break;
            }
            erase_slot(i);
        }
    }

    slot++;
    for(uint16_t i = slot; i < max_slot(); ++i) {
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
    if (find_state(slot, temperature)) {
        erase_slot(slot);
        slot++;
        if (slot >= max_slot()) {
            slot = 0;
        }
    }
    i2cMemoryStateLayout layout;
    layout.marker = 0x00;
    layout.temperature = state.target;
    layout.crc = layout.calculate_crc();
    save_slot(slot, layout);
}
