#include <experimental/optional>
#include "persistent_state.h"

std::experimental::optional<core::PersistentState> getState();
void saveState(core::PersistentState);