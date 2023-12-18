#include "MasterProcess.hpp"

// TODO: delete
#include <iostream>

MasterProcess::MasterProcess(const std::string& fileName, char** env) {
	try {
		CONF::ConfBlock::initInstance(fileName, env);
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterProcess destroyed\n";
	CONF::ConfBlock::getInstance()->destroy();
}

void	MasterProcess::start() {
	CONF::ConfBlock::getInstance()->print();
}