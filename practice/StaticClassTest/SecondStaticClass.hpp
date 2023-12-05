#pragma once

#include <string>

class Second {
private:
    std::string name;

public:
    Second() {name = "Sec";}
    ~Second() {}

    std::string getName() const {
        return this->name;
    }
};