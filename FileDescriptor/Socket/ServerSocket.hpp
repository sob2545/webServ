#pragma once

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
	unsigned short	convertIPv4Address(const std::string& IP);

public:
	ServerSocket(const std::string& IP_Address, const unsigned short& port);
	virtual ~ServerSocket();
};