#include "ClientSocket.hpp"
#include "Exception/SocketException.hpp"
#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include <sys/semaphore.h>
#include <sys/socket.h>


ClientSocket::ClientSocket()
 : FileDescriptor(0)
{}

ClientSocket::~ClientSocket() {}

bool	ClientSocket::accept(const int& serverFd) {
	struct sockaddr_in	addr;
	socklen_t			len = sizeof(addr);

#if	defined (__RELEASE__)
	sem_t*	currSem = ft::Semaphore::getSemaphore(serverFd);
	if (sem_trywait(currSem) == 0) {
#endif
		m_Fd = ::accept(serverFd, reinterpret_cast<struct sockaddr*>(&addr), &len);
		// TODO : need to check
#if	defined (__RELEASE__)
		usleep(100);
		sem_post(currSem);
	} else {
		return (false);		
	}
#endif
	
	if (m_Fd < 0) {
		// TODO: Client SocketException은 에러가 아님 (종료되면 안됨)
		throw SOCK::SocketException("accept() failed");
	}

	setNonBlocking();
	return (true);
}

