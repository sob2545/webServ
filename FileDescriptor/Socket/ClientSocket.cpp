#include "ClientSocket.hpp"
#include <sys/socket.h>

ClientSocket::ClientSocket() : FileDescriptor(0) {}

ClientSocket::ClientSocket(const ClientSocket& other) : FileDescriptor(other.getFd()) {}

ClientSocket&	ClientSocket::operator=(const ClientSocket& other) {
	this->m_Fd = other.m_Fd;
	return (*this);
}

ClientSocket::~ClientSocket() {}
