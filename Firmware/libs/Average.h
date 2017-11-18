#pragma once

#include <array>

namespace libs {

template <typename Type, size_t elements>
class Average {
 private:
    std::array<Type, elements> history;
    int insertion_index;

 public:
    constexpr Average() : insertion_index(0) {
        std::fill(history.begin(), history.end(), 0);
    }

    void put(const Type& value) {
        history[insertion_index++] = value;
        if (insertion_index == elements) {
            insertion_index = 0;
        }
    }

    Type get() {
        auto output = getSum();
        output /= static_cast<Type>(elements);
        return output;
    }

    Type getSum() {
        Type output = 0;
        for (size_t i = 0; i < elements; ++i) {
            output += history[i];
        }
        return output;
    }

    constexpr size_t size() {
        return elements;
    }
};

}  // namespace libs
