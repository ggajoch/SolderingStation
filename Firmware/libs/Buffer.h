#ifndef BUFFER_H
#define BUFFER_H

template<typename Type, size_t maxLength>
class Buffer {
public:
    Buffer() : index(0) {
    }

    void push(const Type & element) {
        array[index++] = element;
    }

    const Type * get() {
        return array;
    }

    size_t size() {
        return index;
    }

    void clear() {
        index = 0;
    }

private:
    size_t index;
    Type array[maxLength];
};


#endif //BUFFER_H
