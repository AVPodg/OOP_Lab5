cmake_minimum_required(VERSION 3.15)
project(DynamicArrayLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Экспортировать compile_commands.json для clangd / cpptools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Автоматическое подключение Google Test
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# Для Windows нужно предотвратить переопределение макросов Windows.h
if(WIN32)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

# Включаем поддержку polymorphic memory resource
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")
endif()

# Основная программа
add_executable(dynamic_array_lab
    src/main.cpp
    src/memory_resource.cpp
    src/dynamic_array.cpp
    src/memory_resource.h
    src/dynamic_array.h
    src/iterator.h
    src/person.h
    src/test_struct.h 
)

target_include_directories(dynamic_array_lab PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Тесты
add_executable(test_dynamic_array
    tests/test_all.cpp
    src/memory_resource.cpp
    src/dynamic_array.cpp
    src/memory_resource.h
    src/dynamic_array.h
    src/iterator.h
    src/person.h
    src/test_struct.h 
)

target_include_directories(test_dynamic_array PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Связывание тестов с Google Test
target_link_libraries(test_dynamic_array PRIVATE GTest::gtest GTest::gtest_main)

# Добавление тестов в CTest
gtest_discover_tests(test_dynamic_array)

# Настройка компилятора
target_compile_features(dynamic_array_lab PRIVATE cxx_std_17)
target_compile_options(dynamic_array_lab PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_compile_features(test_dynamic_array PRIVATE cxx_std_17)
target_compile_options(test_dynamic_array PRIVATE 
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Информация о проекте
message(STATUS "=== Dynamic Array Lab Configuration ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Main executable: dynamic_array_lab")
message(STATUS "Test executable: test_dynamic_array")
message(STATUS "========================================")