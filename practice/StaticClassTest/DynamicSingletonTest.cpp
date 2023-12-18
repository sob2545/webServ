#include "FirstStaticClass.hpp"
#include <cstddef>
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

    std::size_t  i = 10;

    const std::size_t& tmp = i;
    while (i < 15) {
        ++i;
    }
    std::cout << i << " " << tmp << std::endl;
}