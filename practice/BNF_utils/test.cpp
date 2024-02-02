#include "../URIParser.hpp"
#include "../../../Utils/Color.hpp"
#include <fstream>
#include <iostream>

void	printURI(const URI::data& uri) {
	std::cout << "host: " << uri.host << std::endl;
	std::cout << "port: " << uri.port << std::endl;
	std::cout << "path: \n";
	for (std::size_t k = 0; k < uri.absPath.size(); ++k) {
		std::cout << uri.absPath[k] << "\n";
	}
	for (auto it = uri.query.begin(); it != uri.query.end(); ++it) {
		std::cout << it->first << ": ";
		for (auto v_it = it->second.begin(); v_it != it->second.end(); ++v_it) {
			std::cout << *v_it << ((v_it != it->second.end() - 1) ? ", " : "");
		}
		std::cout << "\n";
	}

	std::cout << "fragment: \n";
	std::cout << uri.fragment << std::endl;
	std::cout << RESET << std::endl;
}

int main() {
	std::ifstream	file("test.txt");
	std::string		inputURI;

	if (!file.is_open()) {
		std::cerr << RED << "Error: cannot open file" << RESET << std::endl;
		return 1;
	}
	std::size_t	i(1);
	while (std::getline(file, inputURI)) {
		try {
			URI::data data = URIParser::Parser(inputURI);
			printURI(data);
			// std::string		host;
			// std::size_t			pos(0);
			// URIParser::host(inputURI, pos, host);
			// std::cout << "host : " << host << std::endl;
		}
		catch (const std::string& errorMsg) {
			std::cerr << RED << i << "Test: "  << errorMsg << RESET << "\n" << std::endl;
		}
		i++;
	}
}
