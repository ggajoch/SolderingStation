#ifndef HAL_LIBS_ARRAY_VIEW_H_
#define HAL_LIBS_ARRAY_VIEW_H_

#include <stddef.h>
#include "hal_assert.h"

namespace hal {
namespace libs {

template<typename T>
class array_view final {
 public:
    //
    // Nested types:
    //

    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    using pointer = T *;
    using reference = T &;
    using const_pointer = const T *;
    using const_reference = const T &;

    //
    // Constructors / assignment:
    //

    constexpr array_view() :
            m_pointer { nullptr }, m_size_in_items { 0 } {
    }

    template<typename ArrayType, size_t ArraySize>
    constexpr explicit array_view(ArrayType (&arr)[ArraySize]) :
            m_pointer { arr }, m_size_in_items { ArraySize } {
    }

    template<typename ArrayType, size_t ArraySize>
    constexpr explicit array_view(ArrayType (&&arr)[ArraySize]) = delete;

    template<typename ContainerType>
    constexpr explicit array_view(const ContainerType & container) :
            m_pointer { container.data() }, m_size_in_items { container.size() } {
    }

    template<typename ContainerType>
    constexpr explicit array_view(const ContainerType && container) = delete;

    template<typename ConvertibleType>
    constexpr array_view(ConvertibleType * array_ptr,
            size_type size_in_items) :
            m_pointer { array_ptr }, m_size_in_items { size_in_items } {
    }

    template<typename ConvertibleType>
    constexpr array_view(array_view<ConvertibleType> other) :
            m_pointer { other.data() }, m_size_in_items { other.size() } {
    }

    template<typename ConvertibleType>
    constexpr array_view & operator =(array_view<ConvertibleType> other) {
        m_pointer = other.data();
        m_size_in_items = other.size();
        return *this;
    }

    //
    // Helper methods:
    //

    void reset() {
        m_pointer = nullptr;
        m_size_in_items = 0;
    }

    array_view slice(const size_type offset_in_items) const {
        return slice(offset_in_items, size());
    }

    array_view slice(const size_type start_offset,
            const size_type end_offset) const {
        check_not_null();

        if (end_offset == start_offset) {
            fail_due_to_error(
                    "array_view slice start and end offsets are the same!");
        }
        if (start_offset > end_offset) {
            fail_due_to_error(
                    "array_view slice start offset greater than end offset!");
        }

        size_type slice_size = end_offset - start_offset;
        pointer slice_ptr = const_cast<pointer>(data()) + start_offset;
        pointer end_ptr = const_cast<pointer>(data()) + size();

        if (slice_ptr > end_ptr) {
            fail_due_to_error("array_view slice start offset is out-of-bounds!");
        }
        if (slice_size > (size() - start_offset)) {
            fail_due_to_error("array_view slice is larger than size!");
        }

        return array_view(slice_ptr, slice_size);
    }

    //
    // Data access:
    //

    reference at(const size_type index) const {
        // at() always validates the array_view and index.
        // operator[] uses assert()s that can be disabled if
        // you care more about performance than runtime checking.
        check_not_null();
        if (index >= size()) {
            fail_due_to_error("array_view::at(): index is out-of-bounds!");
        }
        return *(data() + index);
    }

    reference operator[](const size_type index) const {
        // Unlike with at() these checks can be disabled for ultimate performance.
        assert(data() != nullptr);
        assert(index < size());
        return *(data() + index);
    }

    reference front() const {
        check_not_null();
        return *data();
    }

    reference back() const {
        check_not_null();
        return *(data() + size() - 1);
    }

    pointer begin() const {
        return m_pointer;
    }

    pointer end() const {
        return m_pointer + size();
    }

    //
    // Miscellaneous queries:
    //

    constexpr bool empty() const {
        return size() == 0;
    }
    constexpr size_type size() const {
        return m_size_in_items;
    }
    constexpr size_type size_bytes() const {
        return m_size_in_items * sizeof(value_type);
    }
    pointer data() const {
        return m_pointer;
    }

    //
    // Compare against nullptr (test for a null array_view):
    //

    constexpr bool operator ==(nullptr_t) const {
        return this->data() == nullptr;
    }
    constexpr bool operator !=(nullptr_t) const {
        return !(*this == nullptr);
    }

    //
    // Compare for same array pointer and size:
    //

    constexpr bool operator ==(const array_view & other) const {
        return data() == other.data() && size() == other.size();
    }
    constexpr bool operator !=(const array_view & other) const {
        return !(*this == other);
    }

 private:
    void check_not_null() const {
        if (data() == nullptr || size() == 0) {
            fail_due_to_error("array_view pointer is null or size is zero!");
        }
    }

    // Pointer is just a reference to external memory. Not owned by array_view.
    pointer m_pointer;
    size_type m_size_in_items;
};

//
// make_array_view() helpers:
//

template<typename ArrayType, size_t ArraySize>
constexpr auto make_array_view(ArrayType (&arr)[ArraySize]) {
    return array_view<ArrayType> { arr, ArraySize };
}
template<typename ArrayType>
constexpr auto make_array_view(ArrayType * array_ptr,
        const size_t size_in_items) {
    return array_view<ArrayType> { array_ptr, size_in_items };
}
template<typename ContainerType>
constexpr auto make_array_view(const ContainerType & container) {
    return array_view<typename ContainerType::value_type> { container };
}

}  // namespace libs
}  // namespace hal

#endif  // HAL_LIBS_ARRAY_VIEW_H_
