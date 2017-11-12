#include <gtest/gtest.h>
#include <storage/settings.h>
#include <crc8.h>
#include <sleepManager.h>
#include "storage/state.h"
#include "HALmock.h"

struct i2cMemorySettingsLayout {
    uint16_t version;
    core::Settings settings;
    uint8_t crc;

    gsl::span<std::uint8_t> as_span() {
        return gsl::make_span(reinterpret_cast<std::uint8_t *>(this), sizeof(*this));
    }
} __attribute__((packed));

i2cMemorySettingsLayout* ptr = reinterpret_cast<i2cMemorySettingsLayout*>(HAL::Memory::table.data());

TEST(StorageSettings, read_ok) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x01;
    ptr->crc = crc8(ptr->settings.as_span());
    EXPECT_EQ(ptr->crc, 0);
    EXPECT_TRUE(getSettings());
}

TEST(StorageSettings, read_incorrect_crc) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x01;
    ptr->crc = crc8(ptr->settings.as_span())+1;
    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, read_incorrect_version) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x02;
    ptr->crc = crc8(ptr->settings.as_span());
    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, store_and_read) {
    core::Settings settings;
    settings.backlight = 75.5;

    storeSettings(settings);
    auto x = getSettings();
    EXPECT_TRUE(x);
    EXPECT_TRUE(*x == settings);
}

TEST(StorageSettings, crc_corrupt) {
    core::Settings settings;
    settings.backlight = 75.5;

    storeSettings(settings);

    ptr->crc++;

    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, version_corrupt) {
    core::Settings settings;
    settings.backlight = 75.5;

    storeSettings(settings);

    ptr->version++;

    EXPECT_FALSE(getSettings());
}


TEST(Storage, tickSettings) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);
    EXPECT_FALSE(getSettings());

    core::sleepManager::configurationCorrectState = true;
    core::settings.pidParams.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_TRUE(settings_read);

    EXPECT_FLOAT_EQ(settings_read->pidParams.Kp, 75.6);
}

TEST(Storage, tickSettingsShouldntBeSavedWhenConfigurationIsInvalid) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);
    EXPECT_FALSE(getSettings());

    core::sleepManager::configurationCorrectState = false;
    core::settings.pidParams.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_FALSE(settings_read);
}

TEST(Storage, readSettingsInvalidCRC) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    core::storage::read();

    EXPECT_FALSE(core::sleepManager::configurationCorrectState);
}