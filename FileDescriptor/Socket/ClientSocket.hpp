#pragma once

#include <sys/socket.h>

#include "../FileDescriptor.hpp"

class ClientSocket : public FileDescriptor {
private:
	ClientSocket(const ClientSocket& other);
	ClientSocket&	operator=(const ClientSocket& other);

public:
	ClientSocket(const int& serverFd);
	virtual ~ClientSocket();

	const int&	getFd() const;
};