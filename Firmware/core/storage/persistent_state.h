#ifndef SOLDERINGSTATION_PERSISTENTSTATE_H
#define SOLDERINGSTATION_PERSISTENTSTATE_H

#include <cstring>
#include <gsl/span>

namespace core {

struct PersistentState {
    float target;

    bool operator==(const PersistentState &rhs) const {
        return std::memcmp(reinterpret_cast<const char *>(this), reinterpret_cast<const char *>(&rhs), sizeof(PersistentState))
            == 0;
    }

    bool operator!=(const PersistentState &rhs) const {
        return !(*this == rhs);
    }
};

extern PersistentState persistent_state;

}

#endif //SOLDERINGSTATION_PERSISTENTSTATE_H
