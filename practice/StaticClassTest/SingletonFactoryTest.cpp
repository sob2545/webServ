#include "singletonFactoryTest.hpp"
#include "singletonTestClass.hpp"

#include <iostream>

int	main() {
	TestClass::instance(4);
	std::cout << TestClass::getVar() << std::endl;
	TestClass::instance(10);
	std::cout << TestClass::getVar() << std::endl;

	nonParamClass::instance();
	std::cout << nonParamClass::getVar() << std::endl;
}