#pragma once

#include <optional>
#include "persistent_state.h"

std::optional<core::PersistentState> getState();
void saveState(core::PersistentState);
