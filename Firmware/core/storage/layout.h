#ifndef SOLDERINGSTATION_STORAGE_LAYOUT_H
#define SOLDERINGSTATION_STORAGE_LAYOUT_H

#include "crc8.h"

struct i2cMemorySettingsLayout {
    uint16_t version;
    core::Settings settings;
    uint8_t crc;

    gsl::span<std::uint8_t> as_span() {
        return gsl::make_span(reinterpret_cast<std::uint8_t*>(this), sizeof(*this));
    }

    bool crc_match() {
        return calculate_crc() == crc;
    }

    uint8_t calculate_crc() {
        return crc8(settings.as_span());
    }
} __attribute__((packed));
static_assert(std::is_pod<i2cMemorySettingsLayout>::value, "");
static_assert(sizeof(i2cMemorySettingsLayout) == 31, "");


struct i2cMemoryStateLayout {
    uint8_t marker;
    uint16_t temperature;
    uint8_t crc;

    gsl::span<std::uint8_t> as_span() {
        return gsl::make_span(reinterpret_cast<std::uint8_t*>(this), sizeof(*this));
    }

    bool crc_match() {
        return calculate_crc() == crc;
    }

    uint8_t calculate_crc() {
        auto* ptr = reinterpret_cast<uint8_t*>(this);
        return crc8(gsl::span<uint8_t, 3>(ptr, 3));
    }
} __attribute__((packed));
static_assert(std::is_pod<i2cMemoryStateLayout>::value, "");
static_assert(sizeof(i2cMemoryStateLayout) == 4, "");



#endif //SOLDERINGSTATION_STORAGE_LAYOUT_H
