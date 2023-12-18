#pragma once

#include <sys/socket.h>

#include "../FileDescriptor.hpp"

class ClientSocket : public FileDescriptor {
private:

public:
	ClientSocket();
	ClientSocket(const ClientSocket& other);
	ClientSocket&	operator=(const ClientSocket& other);
	virtual ~ClientSocket();
};