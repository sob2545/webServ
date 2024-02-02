#pragma once

#include <sys/socket.h>

#include "../FileDescriptor.hpp"
#include "../../Utils/Semaphore.hpp"

class ClientSocket : public FileDescriptor {
private:
	ClientSocket(const ClientSocket& other);
	ClientSocket&	operator=(const ClientSocket& other);

public:
	ClientSocket();
	virtual ~ClientSocket();

	bool		accept(const int& serverFd);
};