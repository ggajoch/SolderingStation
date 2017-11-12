#ifndef SOLDERINGSTATION_BASE_H
#define SOLDERINGSTATION_BASE_H

#include <gsl/span>
#include <cstring>

namespace core {
namespace base {

template<typename Derived>
struct Pod {
    bool operator==(const Derived &rhs) const {
        return std::memcmp(reinterpret_cast<const char *>(this), reinterpret_cast<const char *>(&rhs), sizeof(Derived))
            == 0;
    }

    bool operator!=(const Derived &rhs) const {
        return !(*this == rhs);
    }

    gsl::span <std::uint8_t> as_span() {
        static_assert(std::is_pod<Derived>::value, "POD violation");
        return gsl::make_span(reinterpret_cast<std::uint8_t *>(this), sizeof(Derived));
    }
};

}
}

#endif //SOLDERINGSTATION_BASE_H
