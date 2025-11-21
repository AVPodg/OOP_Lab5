#include <iostream>
#include "memory_resource.h"
#include "dynamic_array.h"
#include "person.h"  // Добавляем включение заголовка с Person

void demo_simple_types() {
    std::cout << "=== DEMO WITH SIMPLE TYPES (int) ===" << std::endl;
    
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    std::cout << "Adding elements to int array:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        arr.push_back(i * 10);
        std::cout << "Added: " << i * 10 << ", size: " << arr.size() 
                  << ", capacity: " << arr.capacity() << std::endl;
    }
    
    std::cout << "\nIterating through int array:" << std::endl;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nRange-based for loop:" << std::endl;
    for (const auto& elem : arr) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\nAccess by index:" << std::endl;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << "arr[" << i << "] = " << arr[i] << std::endl;
    }
}

void demo_complex_types() {
    std::cout << "\n=== DEMO WITH COMPLEX TYPES (Person) ===" << std::endl;
    
    dynamic_memory_resource mr;
    dynamic_array<Person> people(&mr);
    
    std::cout << "Adding Person objects:" << std::endl;
    people.push_back(Person("Alice", 25, 50000.0));
    people.push_back(Person("Bob", 30, 60000.0));
    people.push_back(Person("Charlie", 35, 70000.0));
    
    std::cout << "\nIterating through Person array:" << std::endl;
    for (const auto& person : people) {
        std::cout << person << std::endl;
    }
    
    std::cout << "\nModifying elements:" << std::endl;
    people[1].age = 31;
    people[1].salary = 65000.0;
    
    std::cout << "After modification:" << std::endl;
    for (const auto& person : people) {
        std::cout << person << std::endl;
    }
}

void demo_memory_reuse() {
    std::cout << "\n=== DEMO MEMORY REUSE ===" << std::endl;
    
    dynamic_memory_resource mr;
    
    {
        std::cout << "Creating and destroying arrays to show memory reuse:" << std::endl;
        
        dynamic_array<int> arr1(&mr);
        for (int i = 0; i < 5; ++i) {
            arr1.push_back(i);
        }
        std::cout << "Array 1 created with " << arr1.size() << " elements" << std::endl;
        
        // arr1 уничтожается здесь, память переходит в free_blocks
    }
    
    {
        dynamic_array<int> arr2(&mr);
        for (int i = 10; i < 15; ++i) {
            arr2.push_back(i);
        }
        std::cout << "Array 2 created with " << arr2.size() << " elements" << std::endl;
        // Должна быть использована память из free_blocks
    }
}

int main() {
    std::cout << "DYNAMIC ARRAY WITH CUSTOM MEMORY RESOURCE DEMO\n" << std::endl;
    
    demo_simple_types();
    demo_complex_types();
    demo_memory_reuse();
    
    std::cout << "\n=== PROGRAM FINISHED ===" << std::endl;
    return 0;
}