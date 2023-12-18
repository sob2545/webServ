#include "Parser/ConfParser/Data/ConfBlock.hpp"
#include "Server/MasterProcess.hpp"
#include <iostream>
#include <unistd.h>

void	a() {
	system("leaks a.out");
}

int main(int ac, char** av, char** env) {
	atexit(a);
	system("clear");

	if (ac != 2) {
		std::cerr << "Error: wrong argument" << std::endl;
		return (1);
	}
	MasterProcess::instance(std::string(av[1]), env);
	
	// server initiate
	// MasterProcess::instance("", NULL).start();
	MasterProcess::start();

	return (0);
}