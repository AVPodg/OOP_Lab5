#pragma once
#include <iostream>
#include <string>

struct TestStruct {
    int id;
    double value;
    std::string name;
    
    TestStruct() : id(0), value(0.0), name("") {}
    TestStruct(int i, double v, const std::string& n) : id(i), value(v), name(n) {}
    
    bool operator==(const TestStruct& other) const {
        return id == other.id && value == other.value && name == other.name;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const TestStruct& ts) {
        os << "TestStruct{id: " << ts.id << ", value: " << ts.value << ", name: " << ts.name << "}";
        return os;
    }
};