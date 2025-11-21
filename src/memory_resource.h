#pragma once
#include <memory_resource>
#include <vector>
#include <cstddef>

class dynamic_memory_resource : public std::pmr::memory_resource {
private:
    struct block_info {
        void* ptr;
        std::size_t size;
    };
    
    std::vector<block_info> allocated_blocks;
    std::vector<block_info> free_blocks;
    
    void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    
public:
    dynamic_memory_resource() = default;
    ~dynamic_memory_resource();
    
    // Запрещаем копирование и перемещение
    dynamic_memory_resource(const dynamic_memory_resource&) = delete;
    dynamic_memory_resource& operator=(const dynamic_memory_resource&) = delete;
};