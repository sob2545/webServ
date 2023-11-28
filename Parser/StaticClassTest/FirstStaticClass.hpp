#pragma once

#include "SecondStaticClass.hpp"

class First {
private:
    std::string     name;
    static Second   secInstance;

    First() {this->name = "First";}
    ~First() {}

public:
    static First*    instance;

    static First*    getInstance() {
        if (!First::instance) {
            First::instance = new First();
        }
        return First::instance;
    }
    static Second   getSecInstance() {return First::secInstance;}
    static void     Destroy() {
        delete First::instance;
    }
    std::string     getFirstName() {return this->name;}
};

First* First::instance = NULL;
Second First::secInstance;