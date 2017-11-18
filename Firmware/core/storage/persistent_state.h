#ifndef SOLDERINGSTATION_PERSISTENTSTATE_H
#define SOLDERINGSTATION_PERSISTENTSTATE_H

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

#endif  // SOLDERINGSTATION_PERSISTENTSTATE_H
