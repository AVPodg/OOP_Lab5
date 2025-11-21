# Dynamic Array with Custom Memory Resource

### О проекте
Проект реализует динамический массив с пользовательским аллокатором памяти, соответствующий требованиям стандарта C++17. Включает кастомный memory resource с переиспользованием памяти, шаблонный контейнер и STL-совместимые итераторы.

### Автор
Подгурский Александр
Группа: М8О-203БВ-24

### Структура проекта
```text
src/
├── memory_resource.h/cpp    # Кастомный аллокатор
├── dynamic_array.h/cpp      # Шаблонный динамический массив  
├── iterator.h               # Итераторы
├── person.h                 # Пример сложного типа
├── test_struct.h            # Структура для тестов
└── main.cpp                 # Демонстрация
tests/
└── test_all.cpp            # Комплексные тесты
```
### Быстрый старт
#### Сборка проекта
```bash
mkdir build
cd build
cmake ..
make
```
#### Запуск демо
```bash
./dynamic_array_lab
```
#### Запуск тестов
```bash
./test_dynamic_array
# или
ctest
```
### Основные компоненты
1. Dynamic Memory Resource
- Наследник std::pmr::memory_resource
- Переиспользование освобожденной памяти
- Автоматическая очистка при разрушении

2. Dynamic Array
- Шаблонный контейнер с std::pmr::polymorphic_allocator
- Автоматическое увеличение емкости
- Поддержка семантики перемещения

3. Итераторы
- Категория: std::forward_iterator_tag
- Поддержка range-based for loops
- Полная совместимость с STL

### Пример использования
```cpp
#include "src/dynamic_array.h"
#include "src/memory_resource.h"

dynamic_memory_resource mr;
dynamic_array<int> arr(&mr);

// Добавление элементов
arr.push_back(1);
arr.push_back(2);
arr.push_back(3);

// Итерация
for (const auto& elem : arr) {
    std::cout << elem << " ";
}
// Работа со сложными типами
dynamic_array<Person> people(&mr);
people.push_back(Person("Alice", 25, 50000.0));
```

### Тестирование
- Проект включает 44 теста, покрывающих:
- Базовые операции аллокатора
- Функциональность динамического массива
- Работу итераторов
- Интеграционные тесты

### Граничные случаи
- Все тесты успешно проходят, демонстрируя корректную работу переиспользования памяти и всех компонентов системы.

### Требования
- Компилятор с поддержкой C++17
- CMake 3.15+
- Google Test (автоматически загружается)
