#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "config.h"
#include "core.h"

// hack to get access to internal type
namespace core {
namespace stateManager {
extern uint8_t config_send_from_pc;
}
}

TEST(Controller, setup) {
    HAL::Tip::heatingPercentage = 10;
    core::setup();
    EXPECT_FLOAT_EQ(HAL::Tip::heatingPercentage, 0);
}

void clear() {
    while (!HAL::Tip::rawTemperatureData.empty())
        HAL::Tip::rawTemperatureData.pop();
}

static void parse(const char* cmd) {
    static char tmp[100];
    strcpy(tmp, cmd);
    HAL::Com::handler(tmp);
}

TEST(Controller, averaging) {
    clear();

    core::settings.tipParams.offset = 0;
    core::settings.tipParams.gain = 1;

    for (int i = 0; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i % 2);
    }
    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 0.5);

    clear();

    HAL::Tip::rawTemperatureData.push(50);
    for (int i = 1; i < core::config::tempAverages; ++i) {
        HAL::Tip::rawTemperatureData.push(i);
        printf("%d ", i);
    }
    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 100);

    core::tick();

    EXPECT_EQ(HAL::Tip::rawTemperatureData.size(), 1); // leftover for testing
    EXPECT_FLOAT_EQ(core::temp, 50.0);
}


TEST(Controller, blank_memory_setup) {
    std::fill(HAL::Memory::table.begin(), HAL::Memory::table.end(), 0xFF);
    HAL::Tip::in_stand = true;
    core::stateManager::config_send_from_pc = 0;

    core::setup();

    parse("temp 10");
    core::tick();

    EXPECT_TRUE(core::stateManager::sleep);
    EXPECT_FALSE(core::stateManager::configuration_correct);
    EXPECT_TRUE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0);
    EXPECT_FLOAT_EQ(core::persistent_state.target, 10.0);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);

    parse("pid 0.1 0.2 0.3 0.4");
    core::tick();

    EXPECT_TRUE(core::stateManager::sleep);
    EXPECT_FALSE(core::stateManager::configuration_correct);
    EXPECT_TRUE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b01);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);

    parse("disp 10.1 20.2");
    core::tick();

    EXPECT_TRUE(core::stateManager::sleep);
    EXPECT_FALSE(core::stateManager::configuration_correct);
    EXPECT_TRUE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b101);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::settings.display.backlight, 10.1);
    EXPECT_FLOAT_EQ(core::settings.display.contrast, 20.2);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);

    parse("tip 1.1 2.2");
    core::tick();

    EXPECT_TRUE(core::stateManager::sleep);
    EXPECT_TRUE(core::stateManager::configuration_correct);
    EXPECT_TRUE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b111);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1.1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2.2);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);

    HAL::Encoder::buttonHandler();
    core::tick();

    EXPECT_FALSE(core::stateManager::sleep);
    EXPECT_TRUE(core::stateManager::configuration_correct);
    EXPECT_TRUE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b111);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1.1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2.2);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);

    HAL::Tip::in_stand = false;
    core::tick();

    EXPECT_FALSE(core::stateManager::sleep);
    EXPECT_TRUE(core::stateManager::configuration_correct);
    EXPECT_FALSE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b111);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1.1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2.2);
    EXPECT_FLOAT_EQ(core::pid.target, 10.0-0.5);

    HAL::Encoder::buttonHandler();
    core::tick();

    EXPECT_TRUE(core::stateManager::sleep);
    EXPECT_TRUE(core::stateManager::configuration_correct);
    EXPECT_FALSE(core::stateManager::in_stand);
    EXPECT_EQ(core::stateManager::config_send_from_pc, 0b111);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kp, 0.1);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Ki, 0.2);
    EXPECT_FLOAT_EQ(core::settings.pidParams.Kd, 0.3);
    EXPECT_FLOAT_EQ(core::settings.pidParams.max_power, 0.4);
    EXPECT_FLOAT_EQ(core::settings.tipParams.offset, 1.1);
    EXPECT_FLOAT_EQ(core::settings.tipParams.gain, 2.2);
    EXPECT_FLOAT_EQ(core::pid.target, 0.0);
}