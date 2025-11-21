#include "dynamic_array.h"
#include "person.h"
#include "test_struct.h"  // Добавляем включение
#include <iostream>
#include <utility>
#include <string>

// Явные инстанциации для нужных типов
template class dynamic_array<int>;
template class dynamic_array<std::string>;
template class dynamic_array<Person>;
template class dynamic_array<double>;
template class dynamic_array<TestStruct>;  // Добавляем для TestStruct

// Явная инстанциация для TestStruct будет добавлена после его определения
// Пока просто объявим, что она будет
// template class dynamic_array<TestStruct>;

template<typename T>
dynamic_array<T>::dynamic_array(std::pmr::memory_resource* mr)
    : data_(nullptr), size_(0), capacity_(0), allocator_(mr) {}

template<typename T>
dynamic_array<T>::dynamic_array(std::size_t initial_size, std::pmr::memory_resource* mr)
    : data_(nullptr), size_(0), capacity_(0), allocator_(mr) {
    resize(initial_size);
    size_ = initial_size;
    
    // Инициализируем элементы
    for (std::size_t i = 0; i < size_; ++i) {
        allocator_.construct(&data_[i], T{});
    }
}

template<typename T>
dynamic_array<T>::~dynamic_array() {
    if (data_) {
        for (std::size_t i = 0; i < size_; ++i) {
            allocator_.destroy(&data_[i]);
        }
        allocator_.deallocate(data_, capacity_);
    }
}

template<typename T>
void dynamic_array<T>::resize(std::size_t new_capacity) {
    if (new_capacity <= capacity_) return;

    T* new_data = allocator_.allocate(new_capacity);
    
    // Перемещаем существующие элементы
    for (std::size_t i = 0; i < size_; ++i) {
        allocator_.construct(&new_data[i], std::move(data_[i]));
        allocator_.destroy(&data_[i]);
    }
    
    if (data_) {
        allocator_.deallocate(data_, capacity_);
    }
    
    data_ = new_data;
    capacity_ = new_capacity;
}

template<typename T>
T& dynamic_array<T>::operator[](std::size_t index) {
    return data_[index];
}

template<typename T>
const T& dynamic_array<T>::operator[](std::size_t index) const {
    return data_[index];
}

template<typename T>
std::size_t dynamic_array<T>::size() const {
    return size_;
}

template<typename T>
std::size_t dynamic_array<T>::capacity() const {
    return capacity_;
}

template<typename T>
bool dynamic_array<T>::empty() const {
    return size_ == 0;
}

template<typename T>
void dynamic_array<T>::push_back(const T& value) {
    if (size_ >= capacity_) {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    allocator_.construct(&data_[size_], value);
    ++size_;
}

template<typename T>
void dynamic_array<T>::push_back(T&& value) {
    if (size_ >= capacity_) {
        resize(capacity_ == 0 ? 1 : capacity_ * 2);
    }
    allocator_.construct(&data_[size_], std::move(value));
    ++size_;
}

template<typename T>
typename dynamic_array<T>::iterator dynamic_array<T>::begin() {
    return iterator(data_);
}

template<typename T>
typename dynamic_array<T>::iterator dynamic_array<T>::end() {
    return iterator(data_ + size_);
}

template<typename T>
typename dynamic_array<T>::const_iterator dynamic_array<T>::begin() const {
    return const_iterator(data_);
}

template<typename T>
typename dynamic_array<T>::const_iterator dynamic_array<T>::end() const {
    return const_iterator(data_ + size_);
}

template<typename T>
typename dynamic_array<T>::const_iterator dynamic_array<T>::cbegin() const {
    return const_iterator(data_);
}

template<typename T>
typename dynamic_array<T>::const_iterator dynamic_array<T>::cend() const {
    return const_iterator(data_ + size_);
}