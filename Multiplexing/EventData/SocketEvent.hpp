#pragma once

#define __DARWIN__
// #define __LINUX__

#if defined (__DARWIN__)

	#include <sys/event.h>

#elif defined (__LINUX__)

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
#if defined (__DARWIN__)

	typedef struct kevent 					Event_t;

#elif defined (__LINUX__)

	typedef struct epoll_event				Event_t;

#endif

private:
	short	m_Status;
	Event_t	m_Event;

public:
	SocketEvent();
	SocketEvent(const Event_t& other);
	SocketEvent(const SocketEvent& other);
	const SocketEvent& operator=(const SocketEvent& other);
	const SocketEvent& operator=(const Event_t& other);
	~SocketEvent();

public:
	const int		getFd() const;
	const short&	getStatus() const;
	bool			isReadEvent() const;
	bool			isWriteEvent() const;
	
};