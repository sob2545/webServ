#pragma once

#include <_types/_uint32_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <string>

#include "../FileDescriptor.hpp"
#include "Exception/SocketException.hpp"

class ServerSocket : public FileDescriptor {
private:
	ServerSocket(const ServerSocket& other);
	ServerSocket&	operator=(const ServerSocket& other);

private:
	uint32_t	IPv4toUL(const std::string& IP);

public:
	ServerSocket(const std::string& IP_Address, const unsigned short& port);
	virtual ~ServerSocket();
};