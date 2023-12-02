#include "../ConfData/ConfBlock.hpp"
#include <iostream>
#include <fstream>

int main() {
	const std::string	fileName = "test.conf";
	try {
		CONF::ConfBlock::initInstance(fileName);
		CONF::ConfBlock::getInstance()->print();
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}