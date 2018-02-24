#include <gtest/gtest.h>
#include "HALmock.h"

#include "stateManager.h"

#include "storage/storage.h"
#include "storage/settings.h"
#include "storage/layout.h"

i2cMemorySettingsLayout* ptr = reinterpret_cast<i2cMemorySettingsLayout*>(HAL::Memory::table.data());

TEST(StorageSettings, read_ok) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x01;
    ptr->crc = ptr->calculate_crc();
    EXPECT_EQ(ptr->crc, 0);
    EXPECT_TRUE(getSettings());
}

TEST(StorageSettings, read_incorrect_crc) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x01;
    ptr->crc = ptr->calculate_crc()+1;
    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, read_incorrect_version) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    ptr->version = 0x02;
    ptr->crc = ptr->calculate_crc();
    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, store_and_read) {
    core::Settings settings;
    settings.display.backlight = 75.5;

    storeSettings(settings);
    auto x = getSettings();
    EXPECT_TRUE(x);
    EXPECT_TRUE(*x == settings);
}

TEST(StorageSettings, crc_corrupt) {
    core::Settings settings;
    settings.display.backlight = 75.5;

    storeSettings(settings);

    ptr->crc++;

    EXPECT_FALSE(getSettings());
}

TEST(StorageSettings, version_corrupt) {
    core::Settings settings;
    settings.display.backlight = 75.5;

    storeSettings(settings);

    ptr->version++;

    EXPECT_FALSE(getSettings());
}


TEST(Storage, tickShouldSaveSettings) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0xFF);
    EXPECT_FALSE(getSettings());

    core::stateManager::state = core::stateManager::State::On;
    core::settings.pid.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_TRUE(settings_read);

    EXPECT_FLOAT_EQ(settings_read->pid.Kp, 75.6);

    // memory unchanged from byte 40
    for(int i = 40; i < HAL::Memory::table.size(); ++i) {
        EXPECT_EQ(HAL::Memory::table.at(i), 0xFF);
    }
}

TEST(Storage, tickSettingsShouldntBeSavedWhenConfigurationIsInvalid) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);
    EXPECT_FALSE(getSettings());

    core::stateManager::state = core::stateManager::State::InvalidConfig;
    core::settings.pid.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_FALSE(settings_read);
}

TEST(Storage, readInvalidSettingsDefaultConfig) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    core::stateManager::state = core::stateManager::State::On;

    core::setup();

    EXPECT_EQ(core::stateManager::State::InvalidConfig, core::stateManager::state);
    EXPECT_FLOAT_EQ(core::settings.pid.Kp, 0);
    EXPECT_FLOAT_EQ(core::settings.pid.Ki, 0);
    EXPECT_FLOAT_EQ(core::settings.pid.Kd, 0);
    EXPECT_FLOAT_EQ(core::settings.tip.offset, 0);
    EXPECT_FLOAT_EQ(core::settings.tip.gain, 0);
    EXPECT_FLOAT_EQ(core::settings.display.contrast, 27);
    EXPECT_FLOAT_EQ(core::settings.display.backlight, 100);
}