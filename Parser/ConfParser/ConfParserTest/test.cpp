#include <iostream>
#include "../ConfParserHeaders/ConfMainBlock.hpp"
#include <fstream>

int main() {
	std::ifstream	file("test.conf");
	std::string		fileContent;

	if (!file.is_open()) {
		std::cout << "File not found" << std::endl;
        return 1;
    }
	std::getline(file, fileContent, '\0');
	file.close();
	try {
		CONF::mainBlock::initInstance(fileContent);
		CONF::mainBlock::getInstance()->print();
	} catch (const std::string& message) {
		std::cerr << message << std::endl;
	}
}