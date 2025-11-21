#pragma once
#include <memory_resource>
#include "iterator.h"

template<typename T>
class dynamic_array {
private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    std::pmr::polymorphic_allocator<T> allocator_;

    void resize(std::size_t new_capacity);

public:
    using iterator = dynamic_array_iterator<T>;
    using const_iterator = dynamic_array_iterator<const T>;

    explicit dynamic_array(std::pmr::memory_resource* mr = std::pmr::get_default_resource());
    dynamic_array(std::size_t initial_size, std::pmr::memory_resource* mr = std::pmr::get_default_resource());
    ~dynamic_array();

    // Запрещаем копирование и перемещение для простоты
    dynamic_array(const dynamic_array&) = delete;
    dynamic_array& operator=(const dynamic_array&) = delete;

    // Доступ к элементам
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    // Размер и емкость
    std::size_t size() const;
    std::size_t capacity() const;
    bool empty() const;

    // Добавление элементов
    void push_back(const T& value);
    void push_back(T&& value);

    // Итераторы
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;
};