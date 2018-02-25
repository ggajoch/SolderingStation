#pragma once

#include <optional>
#include "../settings.h"

std::optional<core::Settings> getSettings();
void storeSettings(core::Settings settings);
