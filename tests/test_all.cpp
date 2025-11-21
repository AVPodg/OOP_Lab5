#include <gtest/gtest.h>
#include "../src/memory_resource.h"
#include "../src/dynamic_array.h"
#include "../src/iterator.h"
#include "../src/person.h"
#include "../src/test_struct.h"  // Включаем вместо локального определения
#include <memory>
#include <string>
#include <algorithm>
#include <vector>

// Убираем локальное определение TestStruct, используем из test_struct.h

// Фикстура для тестов memory_resource
class MemoryResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        mr = std::make_unique<dynamic_memory_resource>();
    }

    void TearDown() override {
        mr.reset();
    }

    std::unique_ptr<dynamic_memory_resource> mr;
};

// Фикстура для тестов dynamic_array
class DynamicArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        mr = std::make_unique<dynamic_memory_resource>();
        arr_int = std::make_unique<dynamic_array<int>>(mr.get());
        arr_string = std::make_unique<dynamic_array<std::string>>(mr.get());
    }

    void TearDown() override {
        arr_int.reset();
        arr_string.reset();
        mr.reset();
    }

    std::unique_ptr<dynamic_memory_resource> mr;
    std::unique_ptr<dynamic_array<int>> arr_int;
    std::unique_ptr<dynamic_array<std::string>> arr_string;
};

// Тесты для memory_resource
TEST_F(MemoryResourceTest, BasicAllocation) {
    void* ptr1 = mr->allocate(100);
    EXPECT_NE(ptr1, nullptr);
    
    void* ptr2 = mr->allocate(50);
    EXPECT_NE(ptr2, nullptr);
    
    mr->deallocate(ptr1, 100);
    mr->deallocate(ptr2, 50);
}

TEST_F(MemoryResourceTest, MemoryReuse) {
    void* ptr1 = mr->allocate(100);
    mr->deallocate(ptr1, 100);
    
    void* ptr2 = mr->allocate(100);
    EXPECT_NE(ptr2, nullptr);
    
    mr->deallocate(ptr2, 100);
}

TEST_F(MemoryResourceTest, DifferentSizes) {
    void* small_ptr = mr->allocate(10);
    void* large_ptr = mr->allocate(1000);
    
    EXPECT_NE(small_ptr, nullptr);
    EXPECT_NE(large_ptr, nullptr);
    EXPECT_NE(small_ptr, large_ptr);
    
    mr->deallocate(small_ptr, 10);
    mr->deallocate(large_ptr, 1000);
}

TEST_F(MemoryResourceTest, IsEqual) {
    dynamic_memory_resource mr2;
    EXPECT_TRUE(mr->is_equal(*mr));
    EXPECT_FALSE(mr->is_equal(mr2));
}

TEST_F(MemoryResourceTest, MultipleAllocationsDeallocations) {
    const int count = 10;
    void* pointers[count];
    
    for (int i = 0; i < count; ++i) {
        pointers[i] = mr->allocate(i * 10 + 10);
        EXPECT_NE(pointers[i], nullptr);
    }
    
    for (int i = 0; i < count; ++i) {
        mr->deallocate(pointers[i], i * 10 + 10);
    }
}

TEST_F(MemoryResourceTest, ZeroSizeAllocation) {
    void* ptr = mr->allocate(0);
    // Для размера 0 может вернуться nullptr, это допустимо
    if (ptr != nullptr) {
        mr->deallocate(ptr, 0);
    }
}

TEST_F(MemoryResourceTest, LargeAllocation) {
    void* ptr = mr->allocate(1000000);
    EXPECT_NE(ptr, nullptr);
    mr->deallocate(ptr, 1000000);
}

TEST_F(MemoryResourceTest, ReuseSmallerBlock) {
    void* ptr1 = mr->allocate(200);
    mr->deallocate(ptr1, 200);
    
    void* ptr2 = mr->allocate(100);
    EXPECT_NE(ptr2, nullptr);
    mr->deallocate(ptr2, 100);
}

TEST_F(MemoryResourceTest, ExactSizeReuse) {
    void* ptr1 = mr->allocate(128);
    mr->deallocate(ptr1, 128);
    
    void* ptr2 = mr->allocate(128);
    EXPECT_NE(ptr2, nullptr);
    mr->deallocate(ptr2, 128);
}

TEST_F(MemoryResourceTest, MultipleReuse) {
    void* ptr = mr->allocate(64);
    mr->deallocate(ptr, 64);
    
    for (int i = 0; i < 5; ++i) {
        void* new_ptr = mr->allocate(64);
        EXPECT_NE(new_ptr, nullptr);
        mr->deallocate(new_ptr, 64);
    }
}

TEST_F(MemoryResourceTest, AllocationFailureHandling) {
    // Тест на обработку очень больших запросов
    // В реальной системе это может вызвать std::bad_alloc
    // Но для теста просто проверяем, что не падает
    EXPECT_NO_THROW({
        void* ptr = mr->allocate(1000000000);
        if (ptr != nullptr) {
            mr->deallocate(ptr, 1000000000);
        }
    });
}

// Тесты для dynamic_array с простыми типами
TEST_F(DynamicArrayTest, DefaultConstructor) {
    EXPECT_EQ(arr_int->size(), 0);
    EXPECT_EQ(arr_int->capacity(), 0);
    EXPECT_TRUE(arr_int->empty());
}

TEST_F(DynamicArrayTest, PushBackInt) {
    arr_int->push_back(42);
    EXPECT_EQ(arr_int->size(), 1);
    EXPECT_FALSE(arr_int->empty());
    EXPECT_EQ((*arr_int)[0], 42);
}

TEST_F(DynamicArrayTest, MultiplePushBack) {
    for (int i = 0; i < 10; ++i) {
        arr_int->push_back(i);
    }
    EXPECT_EQ(arr_int->size(), 10);
    
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ((*arr_int)[i], i);
    }
}

TEST_F(DynamicArrayTest, AccessOperator) {
    arr_int->push_back(1);
    arr_int->push_back(2);
    arr_int->push_back(3);
    
    EXPECT_EQ((*arr_int)[0], 1);
    EXPECT_EQ((*arr_int)[1], 2);
    EXPECT_EQ((*arr_int)[2], 3);
    
    (*arr_int)[1] = 20;
    EXPECT_EQ((*arr_int)[1], 20);
}

TEST_F(DynamicArrayTest, CapacityGrowth) {
    size_t initial_capacity = arr_int->capacity();
    
    for (int i = 0; i < 100; ++i) {
        arr_int->push_back(i);
    }
    
    EXPECT_GT(arr_int->capacity(), initial_capacity);
    EXPECT_EQ(arr_int->size(), 100);
}

TEST_F(DynamicArrayTest, StringElements) {
    arr_string->push_back("hello");
    arr_string->push_back("world");
    arr_string->push_back("test");
    
    EXPECT_EQ(arr_string->size(), 3);
    EXPECT_EQ((*arr_string)[0], "hello");
    EXPECT_EQ((*arr_string)[1], "world");
    EXPECT_EQ((*arr_string)[2], "test");
}

TEST_F(DynamicArrayTest, ModifyStringElements) {
    arr_string->push_back("initial");
    (*arr_string)[0] = "modified";
    EXPECT_EQ((*arr_string)[0], "modified");
}

TEST_F(DynamicArrayTest, LargeNumberOfElements) {
    const int count = 1000;
    for (int i = 0; i < count; ++i) {
        arr_int->push_back(i * 2);
    }
    
    EXPECT_EQ(arr_int->size(), count);
    for (int i = 0; i < count; ++i) {
        EXPECT_EQ((*arr_int)[i], i * 2);
    }
}

TEST_F(DynamicArrayTest, InitialSizeConstructor) {
    dynamic_array<int> arr_with_size(5, mr.get());
    EXPECT_EQ(arr_with_size.size(), 5);
    EXPECT_GE(arr_with_size.capacity(), 5);
}

TEST_F(DynamicArrayTest, PushBackRvalue) {
    std::string str = "temporary";
    arr_string->push_back(std::move(str));
    EXPECT_TRUE(str.empty());
    EXPECT_EQ((*arr_string)[0], "temporary");
}

// Тесты для итераторов
TEST_F(DynamicArrayTest, IteratorBeginEnd) {
    EXPECT_EQ(arr_int->begin(), arr_int->end());
    
    arr_int->push_back(1);
    EXPECT_NE(arr_int->begin(), arr_int->end());
}

TEST_F(DynamicArrayTest, IteratorTraversal) {
    std::vector<int> expected = {1, 2, 3, 4, 5};
    
    for (int val : expected) {
        arr_int->push_back(val);
    }
    
    std::vector<int> actual;
    for (auto it = arr_int->begin(); it != arr_int->end(); ++it) {
        actual.push_back(*it);
    }
    
    EXPECT_EQ(actual, expected);
}

TEST_F(DynamicArrayTest, ConstIterator) {
    arr_int->push_back(10);
    arr_int->push_back(20);
    
    const auto& const_arr = *arr_int;
    auto it = const_arr.begin();
    EXPECT_EQ(*it, 10);
    
    ++it;
    EXPECT_EQ(*it, 20);
}

TEST_F(DynamicArrayTest, RangeBasedForLoop) {
    for (int i = 0; i < 5; ++i) {
        arr_int->push_back(i * 10);
    }
    
    int sum = 0;
    for (const auto& elem : *arr_int) {
        sum += elem;
    }
    
    EXPECT_EQ(sum, 0 + 10 + 20 + 30 + 40);
}

TEST_F(DynamicArrayTest, IteratorPostIncrement) {
    arr_int->push_back(1);
    arr_int->push_back(2);
    
    auto it = arr_int->begin();
    auto old_it = it++;
    
    EXPECT_EQ(*old_it, 1);
    EXPECT_EQ(*it, 2);
}

TEST_F(DynamicArrayTest, IteratorArrowOperator) {
    dynamic_array<std::string> arr(mr.get());
    arr.push_back("test");
    
    auto it = arr.begin();
    EXPECT_EQ(it->size(), 4);
}

TEST_F(DynamicArrayTest, IteratorConversion) {
    arr_int->push_back(42);
    
    dynamic_array<int>::iterator it = arr_int->begin();
    dynamic_array<int>::const_iterator const_it = it;
    
    EXPECT_EQ(*it, *const_it);
}

// Тесты для сложных типов
TEST(DynamicArrayComplexTest, StructElements) {
    dynamic_memory_resource mr;
    dynamic_array<TestStruct> arr(&mr);
    
    arr.push_back(TestStruct(1, 3.14, "first"));
    arr.push_back(TestStruct(2, 2.71, "second"));
    
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0].id, 1);
    EXPECT_EQ(arr[0].value, 3.14);
    EXPECT_EQ(arr[0].name, "first");
    EXPECT_EQ(arr[1].id, 2);
    EXPECT_EQ(arr[1].value, 2.71);
    EXPECT_EQ(arr[1].name, "second");
}

TEST(DynamicArrayComplexTest, ModifyStructElements) {
    dynamic_memory_resource mr;
    dynamic_array<TestStruct> arr(&mr);
    
    arr.push_back(TestStruct(1, 1.0, "original"));
    arr[0].id = 100;
    arr[0].value = 99.9;
    arr[0].name = "modified";
    
    EXPECT_EQ(arr[0].id, 100);
    EXPECT_EQ(arr[0].value, 99.9);
    EXPECT_EQ(arr[0].name, "modified");
}

TEST(DynamicArrayComplexTest, StructIterator) {
    dynamic_memory_resource mr;
    dynamic_array<TestStruct> arr(&mr);
    
    arr.push_back(TestStruct(1, 1.1, "one"));
    arr.push_back(TestStruct(2, 2.2, "two"));
    arr.push_back(TestStruct(3, 3.3, "three"));
    
    int count = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        EXPECT_EQ(it->id, count + 1);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(DynamicArrayComplexTest, StructRangeBasedFor) {
    dynamic_memory_resource mr;
    dynamic_array<TestStruct> arr(&mr);
    
    arr.push_back(TestStruct(1, 1.0, "a"));
    arr.push_back(TestStruct(2, 2.0, "b"));
    
    int sum_ids = 0;
    for (const auto& item : arr) {
        sum_ids += item.id;
    }
    
    EXPECT_EQ(sum_ids, 3);
}

TEST(DynamicArrayComplexTest, PersonType) {
    dynamic_memory_resource mr;
    dynamic_array<Person> people(&mr);
    
    people.push_back(Person("Alice", 25, 50000.0));
    people.push_back(Person("Bob", 30, 60000.0));
    
    EXPECT_EQ(people.size(), 2);
    EXPECT_EQ(people[0].name, "Alice");
    EXPECT_EQ(people[0].age, 25);
    EXPECT_EQ(people[0].salary, 50000.0);
    EXPECT_EQ(people[1].name, "Bob");
    EXPECT_EQ(people[1].age, 30);
    EXPECT_EQ(people[1].salary, 60000.0);
}

// Интеграционные тесты
TEST(IntegrationTest, MemoryReuseBetweenArrays) {
    dynamic_memory_resource mr;
    
    {
        dynamic_array<int> arr1(&mr);
        for (int i = 0; i < 10; ++i) {
            arr1.push_back(i);
        }
    }
    
    {
        dynamic_array<int> arr2(&mr);
        for (int i = 0; i < 10; ++i) {
            arr2.push_back(i * 2);
        }
        EXPECT_EQ(arr2.size(), 10);
        for (int i = 0; i < 10; ++i) {
            EXPECT_EQ(arr2[i], i * 2);
        }
    }
}

TEST(IntegrationTest, MultipleArraysSameResource) {
    dynamic_memory_resource mr;
    
    dynamic_array<int> arr1(&mr);
    dynamic_array<std::string> arr2(&mr);
    dynamic_array<double> arr3(&mr);
    
    arr1.push_back(42);
    arr2.push_back("integration");
    arr3.push_back(3.14159);
    
    EXPECT_EQ(arr1[0], 42);
    EXPECT_EQ(arr2[0], "integration");
    EXPECT_EQ(arr3[0], 3.14159);
}

TEST(IntegrationTest, IteratorWithComplexTypes) {
    dynamic_memory_resource mr;
    dynamic_array<TestStruct> arr(&mr);
    
    std::vector<TestStruct> expected = {
        TestStruct(1, 1.0, "first"),
        TestStruct(2, 2.0, "second"),
        TestStruct(3, 3.0, "third")
    };
    
    for (const auto& item : expected) {
        arr.push_back(item);
    }
    
    auto arr_it = arr.begin();
    auto expected_it = expected.begin();
    
    while (arr_it != arr.end() && expected_it != expected.end()) {
        EXPECT_EQ(arr_it->id, expected_it->id);
        EXPECT_EQ(arr_it->value, expected_it->value);
        EXPECT_EQ(arr_it->name, expected_it->name);
        ++arr_it;
        ++expected_it;
    }
}

TEST(IntegrationTest, StressTest) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    const int operations = 1000;
    for (int i = 0; i < operations; ++i) {
        arr.push_back(i);
        if (i % 100 == 0) {
            EXPECT_EQ(arr[i], i);
        }
    }
    
    EXPECT_EQ(arr.size(), operations);
}

// Тесты на граничные случаи
TEST(EdgeCaseTest, EmptyArrayOperations) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    EXPECT_TRUE(arr.empty());
    EXPECT_EQ(arr.size(), 0);
    EXPECT_EQ(arr.begin(), arr.end());
}

TEST(EdgeCaseTest, SingleElementArray) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    arr.push_back(999);
    
    EXPECT_EQ(arr.size(), 1);
    EXPECT_FALSE(arr.empty());
    EXPECT_NE(arr.begin(), arr.end());
    EXPECT_EQ(*arr.begin(), 999);
}

TEST(EdgeCaseTest, IteratorEquality) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    arr.push_back(1);
    arr.push_back(2);
    
    auto it1 = arr.begin();
    auto it2 = arr.begin();
    
    EXPECT_EQ(it1, it2);
    ++it1;
    EXPECT_NE(it1, it2);
}

TEST(EdgeCaseTest, MoveSemantics) {
    dynamic_memory_resource mr;
    dynamic_array<std::string> arr(&mr);
    
    std::string large_string(1000, 'x');
    arr.push_back(std::move(large_string));
    
    EXPECT_TRUE(large_string.empty());
    EXPECT_EQ(arr[0].size(), 1000);
}

TEST(EdgeCaseTest, ReverseIterationByIndex) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    for (int i = 0; i < 5; ++i) {
        arr.push_back(i);
    }
    
    std::vector<int> forward;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        forward.push_back(*it);
    }
    
    std::vector<int> backward;
    for (size_t i = arr.size(); i > 0; --i) {
        backward.push_back(arr[i - 1]);
    }
    
    std::reverse(backward.begin(), backward.end());
    EXPECT_EQ(forward, backward);
}

// Тест производительности
TEST(PerformanceTest, LargeScaleOperations) {
    dynamic_memory_resource mr;
    dynamic_array<int> arr(&mr);
    
    const int count = 1000; // Уменьшили для скорости тестов
    for (int i = 0; i < count; ++i) {
        arr.push_back(i);
    }
    
    EXPECT_EQ(arr.size(), count);
    
    for (int i = 0; i < count; ++i) {
        EXPECT_EQ(arr[i], i);
    }
}

TEST(PerformanceTest, MemoryEfficiency) {
    dynamic_memory_resource mr;
    
    const int iterations = 10; // Уменьшили для скорости тестов
    for (int iter = 0; iter < iterations; ++iter) {
        dynamic_array<int> arr(&mr);
        for (int i = 0; i < 100; ++i) {
            arr.push_back(i);
        }
    }
}

// Главная функция
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}