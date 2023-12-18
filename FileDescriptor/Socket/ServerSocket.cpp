#include "ServerSocket.hpp"
#include <sys/socket.h>

ServerSocket::ServerSocket() : FileDescriptor(0) {}

ServerSocket::ServerSocket(const ServerSocket& other) : FileDescriptor(other.getFd()) {}

ServerSocket&	ServerSocket::operator=(const ServerSocket& other) {
	this->m_Fd = other.m_Fd;
	return (*this);
}

ServerSocket::~ServerSocket() {}
