#include "MultiplexHandler.hpp"
#include <sys/event.h>

// TODO: delete
#ifdef DEBUG
#include <iostream>
#include "../Utils/Color.hpp"
#endif

#ifdef __DARWIN__

MultiplexHandler::EventVector_t	MultiplexHandler::m_ChangeList;

#endif

MultiplexHandler::MultiplexHandler()
: FileDescriptor(0),
  m_MaxEvents(1024)
{
#if defined	(__DARWIN__)

	this->m_Fd = kqueue();

#elif defined (__LINUX__)

		this->m_Fd = epoll_create(1);

#endif
	if (this->m_Fd < 0) {
		// TODO: have to implement
		throw ;
	}
}

MultiplexHandler::~MultiplexHandler()
{}

/*
 *	mod : READ, WRITE
 *	status : ADD, DELETE, 
 *	type : server socekt / client socket
*/
void	MultiplexHandler::addServerEvent(const int& fd) {
#if defined	(__DARWIN__)

	SocketEvent::Event_t	newEvent;
	EV_SET(&newEvent, fd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
	m_ChangeList.push_back(newEvent);

#elif defined (__LINUX__)

	SocketEvent::Event_t	newEvent;
	newEvent.events = EPOLLIN | EPOLLET;
	newEvent.data.fd = fd;

	if (epoll_ctl(this->m_Fd, EPOLL_CTL_ADD, fd, &newEvent) < 0) {
		throw ; // TODO: implement
	}

#endif
}

void	MultiplexHandler::addClientEvent(const int& fd, const short& status) {
#ifdef DEBUG	
	std::cout << fd << "client event setter: " << status << std::endl;
#endif


	switch (status) {
		case (E_EV::READ): {
#if defined	(__DARWIN__)

			SocketEvent::Event_t	newEvent;
			EV_SET(&newEvent, fd, EVFILT_READ, EV_ADD | EV_ONESHOT | EV_CLEAR, 0, 0, NULL);
			m_ChangeList.push_back(newEvent);


#elif defined (__LINUX__)

			SocketEvent::Event_t	newEvent;
			newEvent.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
			newEvent.data.fd = fd;

			if (epoll_ctl(this->m_Fd, EPOLL_CTL_ADD, fd, &newEvent) < 0) {
				throw ; // TODO: implement
			}

#endif
			break;
		}
		case (E_EV::WRITE): {
#if defined	(__DARWIN__)

			SocketEvent::Event_t	newEvent;
			EV_SET(&newEvent, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT | EV_CLEAR, 0, 0, NULL);
			m_ChangeList.push_back(newEvent);

#elif defined (__LINUX__)

			SocketEvent::Event_t	newEvent;
			newEvent.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
			newEvent.data.fd = fd;

			if (epoll_ctl(MultiplexHandler::instance().getFd(), EPOLL_CTL_ADD, fd, &newEvent) < 0) {
				throw ; // TODO: implement
			}

#endif
		}
	}
}


MultiplexHandler::SocketEventVector_t		MultiplexHandler::eventHandler(const MultiplexHandler::EventTimer_t* timeout) {
	SocketEventVector_t		res;
	int						eventCount(0);
	SocketEvent::Event_t	newEvents[1024];

#if defined	(__DARWIN__)

	eventCount = kevent(MultiplexHandler::instance().getFd(), &m_ChangeList[0], m_ChangeList.size(), &newEvents[0], MultiplexHandler::instance().getMaxEvent(), timeout);
	m_ChangeList.clear();

#elif defined (__LINUX__)

	eventCount = epoll_wait(MultiplexHandler::instance().getFd(), newEvent, MultiplexHandler::instance().getMaxEvent(), *timeout);

#endif

#ifdef DEBUG
	std::cout << BOLDRED << "kevent number: " << eventCount << std::endl << RESET;
#endif

	if (eventCount < 0) {
		throw ;// TODO: implement
	}

	for (int i(0); i < eventCount; ++i) {
		res.push_back(newEvents[i]);
	}
	return res;
}

const int&	MultiplexHandler::getFd() const {
	return this->m_Fd;
}

const int&	MultiplexHandler::getMaxEvent() const {
	return this->m_MaxEvents;
}
