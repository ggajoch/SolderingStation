#pragma once

#include <cstring>
#include <gsl/span>

namespace core {

struct PersistentState {
    uint16_t target;

    bool operator==(const PersistentState& rhs) const {
        return std::memcmp(reinterpret_cast<const char*>(this), reinterpret_cast<const char*>(&rhs), sizeof(PersistentState)) == 0;
    }

    bool operator!=(const PersistentState& rhs) const {
        return !(*this == rhs);
    }
};

}  // namespace core
