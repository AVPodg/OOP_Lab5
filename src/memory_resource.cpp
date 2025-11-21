#include "memory_resource.h"
#include <iostream>
#include <algorithm>

void* dynamic_memory_resource::do_allocate(std::size_t bytes, std::size_t /*alignment*/) {
    // Сначала пытаемся найти подходящий блок в свободных блоках
    auto it = std::find_if(free_blocks.begin(), free_blocks.end(),
        [bytes](const block_info& block) {
            return block.size >= bytes;
        });
    
    if (it != free_blocks.end()) {
        void* ptr = it->ptr;
        allocated_blocks.push_back({ptr, bytes});
        free_blocks.erase(it);
        std::cout << "Reused block: " << ptr << " size: " << bytes << std::endl;
        return ptr;
    }
    
    // Если подходящего свободного блока нет, выделяем новый
    void* ptr = ::operator new(bytes);
    allocated_blocks.push_back({ptr, bytes});
    std::cout << "Allocated new block: " << ptr << " size: " << bytes << std::endl;
    return ptr;
}

void dynamic_memory_resource::do_deallocate(void* p, std::size_t /*bytes*/, std::size_t /*alignment*/) {
    // Находим блок в allocated_blocks и перемещаем его в free_blocks
    auto it = std::find_if(allocated_blocks.begin(), allocated_blocks.end(),
        [p](const block_info& block) {
            return block.ptr == p;
        });
    
    if (it != allocated_blocks.end()) {
        free_blocks.push_back(*it);
        allocated_blocks.erase(it);
        std::cout << "Deallocated block: " << p << " moved to free list" << std::endl;
    }
}

bool dynamic_memory_resource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

dynamic_memory_resource::~dynamic_memory_resource() {
    std::cout << "Cleaning up memory resource..." << std::endl;
    
    // Освобождаем все выделенные блоки
    for (const auto& block : allocated_blocks) {
        std::cout << "Cleaning up allocated block: " << block.ptr << std::endl;
        ::operator delete(block.ptr);
    }
    
    // Освобождаем все свободные блоки
    for (const auto& block : free_blocks) {
        std::cout << "Cleaning up free block: " << block.ptr << std::endl;
        ::operator delete(block.ptr);
    }
    
    allocated_blocks.clear();
    free_blocks.clear();
}