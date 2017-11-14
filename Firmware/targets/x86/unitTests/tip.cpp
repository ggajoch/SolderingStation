#include <stateManager.h>
#include "gtest/gtest.h"

#include "HALmock.h"
#include "config.h"
#include "core.h"

TEST(Tip, in_stand) {
    HAL::Tip::in_stand = true;
    core::tick();
    EXPECT_TRUE(core::stateManager::in_stand);

    HAL::Tip::in_stand = false;
    core::tick();
    EXPECT_FALSE(core::stateManager::in_stand);
}