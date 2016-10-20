//
// Created by gregg on 3/28/2016.
//

#ifndef SOLDER_ARRAYHOLDER_H
#define SOLDER_ARRAYHOLDER_H

#include <array>

template<typename T, size_t max_length>
class ArrayHolder : public std::array<T, max_length> {
public:
    ArrayHolder() {
        _length = 0;
    }

    template<size_t LEN>
    ArrayHolder(ArrayHolder<T, LEN> array) {
        for(int i = 0; i < array.size(); ++i) {
            this->_M_elems[i] = array[i];
        }
        this->_length = array.size();
    }

    ArrayHolder(T * ptr, size_t len) {
        for(int i = 0; i < len; ++i) {
            this->_M_elems[i] = ptr[i];
        }
        this->_length = len;
    }

    size_t size() {
        return _length;
    }

    void append(const T & elem) {
        this->_M_elems[_length++] = elem;
    }
private:
    size_t _length;
};

#endif //SOLDER_ARRAYHOLDER_H
