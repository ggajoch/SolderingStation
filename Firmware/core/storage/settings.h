#pragma once

#include <experimental/optional>
#include "../settings.h"

std::experimental::optional<core::Settings> getSettings();
void storeSettings(core::Settings settings);
