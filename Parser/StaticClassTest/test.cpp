#include "FirstStaticClass.hpp"
#include <iostream>

void    print() {
    std::cout << First::getInstance()->getFirstName() << std::endl;
}

int main() {
    print();
}