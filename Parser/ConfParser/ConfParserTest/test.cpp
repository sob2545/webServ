#include <iostream>
#include "../ConfParserHeaders/ConfMainBlock.hpp"
#include <fstream>

int main() {
	const std::string	fileName = "test.conf";
	try {
		CONF::mainBlock::initInstance(fileName);
		CONF::mainBlock::getInstance()->print();
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}