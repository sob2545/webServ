#include "../Data/ConfBlock.hpp"
#include <iostream>

int main(int ac, char** av, char** env) {
	(void)ac;
	(void)av;
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