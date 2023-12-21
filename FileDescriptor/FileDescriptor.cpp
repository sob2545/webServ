#include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor(int fd) : m_Fd(fd) {}

FileDescriptor::~FileDescriptor() { close(m_Fd); }

const int& FileDescriptor::getFd() const { return m_Fd; }

void FileDescriptor::setNonBlocking() {
	int flags = fcntl(m_Fd, F_GETFL, 0);
	if (flags == -1) {
		//TODO: error handling in FD
		// throw std::runtime_error("fcntl() failed");
	}
	if (fcntl(m_Fd, F_SETFL, flags | O_NONBLOCK) == -1) {
		//TODO: error handling in FD
		// throw std::runtime_error("fcntl() failed");
	}
}