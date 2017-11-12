#ifndef SOLDERINGSTATION_STORAGE_LAYOUT_H
#define SOLDERINGSTATION_STORAGE_LAYOUT_H

#include "crc8.h"
#include "settings.h"

struct i2cMemorySettingsLayout : core::base::Pod<i2cMemorySettingsLayout> {
    uint16_t version;
    core::Settings settings;
    uint8_t crc;
    uint8_t padding;

    bool crc_match() {
        return calculate_crc() == crc;
    }

    uint8_t calculate_crc() {
        return crc8(settings.as_span());
    }
} __attribute__((packed));


struct i2cMemoryStateLayout : core::base::Pod<i2cMemoryStateLayout> {
    uint8_t marker;
    uint16_t temperature;
    uint8_t crc;

    bool crc_match() {
        return calculate_crc() == crc;
    }

    uint8_t calculate_crc() {
        auto* ptr = reinterpret_cast<uint8_t*>(this);
        return crc8(gsl::span<uint8_t, 3>(ptr, 3));
    }
} __attribute__((packed));
static_assert(sizeof(i2cMemoryStateLayout) == 4, "");



#endif //SOLDERINGSTATION_STORAGE_LAYOUT_H
