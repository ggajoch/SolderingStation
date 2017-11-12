#include <gtest/gtest.h>
#include "HALmock.h"

#include "stateManager.h"

#include "storage/storage.h"
#include "storage/settings.h"
#include "storage/state.h"
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

    core::stateManager::configuration_correct = true;
    core::settings.pidParams.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_TRUE(settings_read);

    EXPECT_FLOAT_EQ(settings_read->pidParams.Kp, 75.6);
}

TEST(Storage, tickSettingsShouldntBeSavedWhenConfigurationIsInvalid) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);
    EXPECT_FALSE(getSettings());

    core::stateManager::configuration_correct = false;
    core::settings.pidParams.Kp = 75.6;

    core::storage::tick();

    auto settings_read = getSettings();
    EXPECT_FALSE(settings_read);
}

TEST(Storage, readSettingsInvalidCRC) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0);

    core::stateManager::configuration_correct = true;

    core::setup();

    EXPECT_FALSE(core::stateManager::configuration_correct);
}