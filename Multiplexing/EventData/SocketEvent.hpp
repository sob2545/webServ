#pragma once

#define __DARWIN__
// #define __LINUX__

#ifdef	__DARWIN__

	#include <sys/event.h>

#endif
#ifdef	__LINUX__

	#include <sys/epoll.h>

#endif

namespace E_EV {
	enum E_STATUS : short {
		READ,
		WRITE,
	};
}


class SocketEvent {
public:
#ifdef	__DARWIN__

	typedef struct kevent 					s_event;

#endif
#ifdef	__LINUX__

	typedef struct epoll_event				s_event;

#endif

private:
	short	m_Status;
	s_event	m_Event;

public:
	SocketEvent();
	SocketEvent(const s_event& other);
	SocketEvent(const SocketEvent& other);
	const SocketEvent& operator=(const SocketEvent& other);
	const SocketEvent& operator=(const s_event& other);
	~SocketEvent();

public:
	const int		getFd() const;
	const short&	getStatus() const;
	
};