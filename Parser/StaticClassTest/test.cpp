#include "FirstStaticClass.hpp"
#include <iostream>

void    print() {
    std::cout << First::getInstance()->getFirstName() << std::endl;
    std::cout << First::getInstance()->getSecInstance().getName() << std::endl;
}

int main() {
    print();
    std::cout << First::getInstance()->getFirstName() << std::endl;
    std::cout << First::getInstance()->getSecInstance().getName() << std::endl;
    First::Destroy();
}