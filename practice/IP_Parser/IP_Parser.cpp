#include <iostream>
#include <string>


void	splitPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	std::string	portStr;

	while (pos < inputURI.size()
			&& inputURI[pos] != ' '
			&& inputURI[pos] != ';'
			&& inputURI[pos] != '\n'
			&& inputURI[pos] != '\t') {
		portStr += inputURI.at(pos);
		pos++;
	}
	char*		endPos;
	const long	portNum = strtol(portStr.c_str(), &endPos, 10);
	if (*endPos != '\0' || portStr.size() > 5 || portNum < 0 || portNum > 65535) {
		std::cerr << "Invalid port: " << portStr << std::endl;
	}
	port = static_cast<unsigned short>(portNum) == 0 ? 80 : static_cast<unsigned short>(portNum);
}

bool	setPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == ':') {
		pos++;
		splitPort(inputURI, pos, port);
		return true;
	}
	else {
		splitPort(inputURI, pos, port);
		return false;
	}
}


bool	splitAddressNumber(const std::string& inputURI, std::size_t& pos, std::string& host) {
	if (pos >= inputURI.size() || !std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		return false;
	}
	host += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		host += inputURI.at(pos);
		pos++;
	}
	char*	endptr;
	const unsigned long addressPart = std::strtoul(host.c_str(), &endptr, 10);
	return ((addressPart <= 255) ? true : false );
}

bool	IPv4address(const std::string& inputURI, std::size_t& pos, std::string& host) {
	const std::size_t	startPos = pos;

	for (int i = 0; i < 3;) {
		if (!splitAddressNumber(inputURI, pos, host)) {
			host.clear();
			pos = startPos;
			return false;
		}
		if (pos >= inputURI.size() || inputURI.at(pos) != '.') {
			host.clear();
			pos = startPos;
			return false;
		} else if (pos < inputURI.size() && inputURI.at(pos) == '.') {
			host += '.';
			i++;
			pos++;
		}
	}
	if (!splitAddressNumber(inputURI, pos, host))
		return false;
	return true;
}

bool	IPv4Parser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port) {
	const std::size_t	startPos = pos;

	(IPv4address(inputURI, pos, argument)) ? 0 : pos = startPos;
	return (setPort(inputURI, pos, port));
}

int	main() {
	std::string	argument;
	unsigned short	port;
	std::size_t		pos = 0;

	std::string		inputURI = "127.0.0.1";
	IPv4Parser(inputURI, pos, argument, port);
	std::cout << argument << std::endl;
	std::cout << port << std::endl;

}
