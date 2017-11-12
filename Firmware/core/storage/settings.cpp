#include "settings.h"
#include "config.h"
#include "HAL.h"

#include "layout.h"

std::experimental::optional<core::Settings> getSettings() {
    i2cMemorySettingsLayout settings;

    HAL::Memory::get(0x0000, settings.as_span());

    if (settings.crc_match() && settings.version == core::config::memory_version) {
        auto result = settings.settings;
        return result;
    } else {
        return {};
    }
}

void storeSettings(core::Settings settings) {
    i2cMemorySettingsLayout layout;
    layout.version = core::config::memory_version;
    layout.settings = settings;
    layout.crc = layout.calculate_crc();

    HAL::Memory::set(0x0000, layout.as_span());
}
