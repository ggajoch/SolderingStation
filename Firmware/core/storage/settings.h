#ifndef SOLDERINGSTATION_STORAGE_SETTINGS_H
#define SOLDERINGSTATION_STORAGE_SETTINGS_H

#include <experimental/optional>
#include "../settings.h"

std::experimental::optional<core::Settings> getSettings();
void storeSettings(core::Settings settings);

#endif