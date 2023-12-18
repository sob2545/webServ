#pragma once

#include <sys/socket.h>
#include <string>

#include "../FileDescriptor.hpp"

class ServerSocket : public FileDescriptor {
private:
	ServerSocket(const ServerSocket& other);
	ServerSocket&	operator=(const ServerSocket& other);

public:
	ServerSocket(const unsigned short& port, const std::string& IP_Address);
	virtual ~ServerSocket();
};