#pragma once

#include <sys/socket.h>

#include "../FileDescriptor.hpp"

class ServerSocket : public FileDescriptor {
private:

public:
	ServerSocket();
	ServerSocket(const ServerSocket& other);
	ServerSocket&	operator=(const ServerSocket& other);
	virtual ~ServerSocket();
};