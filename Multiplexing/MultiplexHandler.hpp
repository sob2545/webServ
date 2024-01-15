#pragma once

#include "../Utils/Singleton.hpp"
#include "../FileDescriptor/FileDescriptor.hpp"
#include "./EventData/SocketEvent.hpp"
#include <ctime>
#include <vector>

#define __DARWIN__
// #define __LINUX__


class MultiplexHandler : public Singleton<MultiplexHandler>, FileDescriptor {
public:
	// 실제 클라이언트가 순회하면서 확인할 event 리스트 (이름 변경 필요)
	typedef std::vector<SocketEvent>		SocketEventVector_t;

#if defined (__DARWIN__)

	typedef	std::vector<struct kevent>		EventVector_t;
	typedef struct timespec					EventTimer_t;

#elif defined (__LINUX__)

	typedef std::vector<struct epoll_event>	EventVector_t;
	typedef int								EventTimer_t;

#endif

private:
	MultiplexHandler(const MultiplexHandler& other);
	const MultiplexHandler& operator=(const MultiplexHandler& other);

private:
#ifdef __DARWIN__

	static EventVector_t	m_ChangeList;

#endif
	const int	m_MaxEvents;

private:
	const int&	getFd() const;
	const int&	getMaxEvent() const;

public:
	MultiplexHandler();
	~MultiplexHandler();


	static void					addServerEvent(const int& fd);
	static void					addClientEvent(const int& fd, const short& status);
	static SocketEventVector_t	eventHandler(const EventTimer_t* timeout);

};