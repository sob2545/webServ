#include "../Data/ConfBlock.hpp"
#include <cstdlib>
#include <iostream>
#include <unistd.h>

// void	a() {
// 	system("leaks a.out");
// }

int main(int ac, char** av, char** env) {
	// atexit(a);
	(void)ac;
	(void)av;
	system("clear");

	const std::string	fileName = "test.conf";
	try {
		CONF::ConfBlock::initInstance(fileName, env);
		CONF::ConfBlock::getInstance()->print();
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	//CONF::ConfBlock::getInstance()->destroy();
}