#include "../Data/ConfBlock.hpp"
#include <iostream>

int main(int ac, char** av, char** env) {
	const std::string	fileName = "test.conf";
	try {
		CONF::ConfBlock::initInstance(fileName, env);
		CONF::ConfBlock::getInstance()->print();
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}