#pragma once
#include <iostream>
#include <string>

struct Person {
    std::string name;
    int age;
    double salary;
    
    Person() : name(""), age(0), salary(0.0) {}
    Person(const std::string& n, int a, double s) : name(n), age(a), salary(s) {}
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << "Person{name: " << p.name << ", age: " << p.age << ", salary: " << p.salary << "}";
        return os;
    }
};