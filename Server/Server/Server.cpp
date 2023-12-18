#include "Server.hpp"

Server::Server(const ft::shared_ptr<CONF::ServerBlock>& block)
  : m_ServerBlock(block),
{
	
}

Server::~Server() {

}


unsigned short	Server::convertIPv4Address(const std::string& IP) {
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

	return htonl(addrNumber);
}