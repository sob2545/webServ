#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <exception>


std::string	errorMessageGenerator(const std::string& inputURI, const int pos) {
	std::stringstream	res;
	res << "Error:" << pos << " \"" << inputURI << "\" is invalid scheme syntax";
	return res.str();
} 

bool	isValidSchemeSyntax(const std::string& inputURI, size_t& pos) {
	return (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == '+' || inputURI.at(pos) == '-' || inputURI.at(pos) == '.');
}

std::string	scheme(const std::string& inputURI, size_t& pos) {
	const std::string ref_scheme = "https ftp";
	std::string	res;

	if (std::isalpha(inputURI.at(pos))) {
		res += inputURI[pos++];
		while (pos < inputURI.size() && isValidSchemeSyntax(inputURI, pos)) {
			res += inputURI[pos];
			pos++;
		}
	}
	try {
		if (res.empty() || ref_scheme.find(res) == std::string::npos) {
			throw errorMessageGenerator(inputURI, pos);
		}
	} catch (const std::string& errorMsg) {
		std::cerr << errorMsg << std::endl;
	}
	return res;
}

bool	absoluteURI(const std::string& inputURI, size_t& pos) {
	std::string	print_scheme = scheme(inputURI, pos);
	std::cout << print_scheme << " ";
	return true;
}

void	parse(const std::string& inputURI) {
	// absoluteURI | relativeURI  [# fragment ]
	size_t	pos(0);

	absoluteURI(inputURI, pos);

	// fragment();
}

int main() {
	std::string inputURI;
	std::cin >> inputURI;

	parse(inputURI);
}
