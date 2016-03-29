#ifndef AVG_H_
#define AVG_H_

template<typename Type, size_t elements>
class Average {
private:
    std::array<Type, elements> history;
    int insertion_index;

public:
    Average() : insertion_index(0) {
        for(auto &elem : history)
            elem = 0;
    }

    void put(const Type &value) {
        history[insertion_index++] = value;
        if (insertion_index == elements)
            insertion_index = 0;
    }

    Type get() {
        Type output = 0.0;
        for (unsigned int i = 0; i < elements; ++i) {
            output += history[i];
        }
        output /= static_cast<Type>(elements);
        return output;
    }
};

#endif // AVG_H_
