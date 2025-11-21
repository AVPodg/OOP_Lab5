#pragma once
#include <iterator>

template<typename T>
class dynamic_array_iterator {
private:
    T* ptr;

public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    dynamic_array_iterator(T* p = nullptr) : ptr(p) {}

    // Dereference
    reference operator*() const { return *ptr; }
    pointer operator->() const { return ptr; }

    // Prefix increment
    dynamic_array_iterator& operator++() {
        ++ptr;
        return *this;
    }

    // Postfix increment
    dynamic_array_iterator operator++(int) {
        dynamic_array_iterator temp = *this;
        ++ptr;
        return temp;
    }

    // Comparison operators
    bool operator==(const dynamic_array_iterator& other) const {
        return ptr == other.ptr;
    }

    bool operator!=(const dynamic_array_iterator& other) const {
        return ptr != other.ptr;
    }

    // Conversion for const_iterator
    operator dynamic_array_iterator<const T>() const {
        return dynamic_array_iterator<const T>(ptr);
    }
};