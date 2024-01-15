#include "ClientSocket.hpp"
#include "Exception/SocketException.hpp"
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>

ClientSocket::ClientSocket(const int& serverFd)
 : FileDescriptor(0)
 {
	struct sockaddr_in	addr;
	socklen_t			len = sizeof(addr);

	m_Fd = accept(serverFd, reinterpret_cast<struct sockaddr*>(&addr), &len);
	if (m_Fd < 0) {
		throw SOCK::SocketException("accept() failed");
	}

	setNonBlocking();
 }

ClientSocket::~ClientSocket() {}

const int&	ClientSocket::getFd() const {
	return this->m_Fd;
}
