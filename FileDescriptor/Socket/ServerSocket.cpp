#include "ServerSocket.hpp"
#include "Exception/SocketException.hpp"

//TODO: delete
#include <_types/_uint32_t.h>
#include <iostream>

ServerSocket::ServerSocket(const std::string& IP_Address, const unsigned short& port)
 : FileDescriptor(0)
{
	struct sockaddr_in	addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(convertIPv4Address(IP_Address));

	if ((m_Fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		throw SOCK::SocketException("socket() failed");
	}

	if (bind(m_Fd, reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) == -1) {
		std::cerr << "listen() failed: " << strerror(errno) << std::endl;
		throw SOCK::SocketException("bind() failed");
	}

	if (listen(m_Fd, 10) == -1) {
		throw SOCK::SocketException("listen() failed");
	}
}

ServerSocket::~ServerSocket() {}


uint32_t	ServerSocket::convertIPv4Address(const std::string& IP) {
	std::stringstream	ipStream(IP);
	std::string			segment;
	uint32_t			addrNumber = 0;

	if (IP.empty()) {
		return (INADDR_ANY);
	}
	for (int i = 0; i < 4; i++) {
		std::getline(ipStream, segment, '.');

		const int segmentNumber = std::atoi(segment.c_str());
		addrNumber |= (segmentNumber << ((3 - i) * 8));
	}
	return (addrNumber);
}