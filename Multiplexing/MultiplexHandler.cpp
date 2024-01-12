#include "MultiplexHandler.hpp"

#ifdef __DARWIN__

MultiplexHandler::eventList	MultiplexHandler::m_ChangeList;

#endif

MultiplexHandler::MultiplexHandler()
: FileDescriptor(0),
  m_MaxEvents(1024)
{
#ifdef __DARWIN__

	this->m_Fd = kqueue();

#endif
#ifdef __LINUX__

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
#ifdef __DARWIN__

	SocketEvent::s_event	newEvent;
	EV_SET(&newEvent, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
	m_ChangeList.push_back(newEvent);

#endif
#ifdef	__LINUX__

	SocketEvent::s_event	newEvent;
	newEvent.events = EPOLLIN | EPOLLET;
	newEvent.data.fd = fd;

	if (epoll_ctl(this->m_Fd, EPOLL_CTL_ADD, fd, &newEvent) < 0) {
		throw ; // TODO: implement
	}

#endif
}

void	MultiplexHandler::addClientEvent(const int& fd, const short& status) {
	switch (status) {
		case (E_EV::E_STATUS::READ): {
#ifdef __DARWIN__

			SocketEvent::s_event	newEvent;
			EV_SET(&newEvent, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
			m_ChangeList.push_back(newEvent);

#endif
#ifdef	__LINUX__

			SocketEvent::s_event	newEvent;
			newEvent.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
			newEvent.data.fd = fd;

			if (epoll_ctl(this->m_Fd, EPOLL_CTL_ADD, fd, &newEvent) < 0) {
				throw ; // TODO: implement
			}

#endif
		}
		case (E_EV::E_STATUS::WRITE): {
#ifdef __DARWIN__

			SocketEvent::s_event	newEvent;
			EV_SET(&newEvent, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, NULL);
			m_ChangeList.push_back(newEvent);

#endif
#ifdef	__LINUX__

			SocketEvent::s_event	newEvent;
			newEvent.events = EPOLLOUT | EPOLLET | EPOLLONESHOT;
			newEvent.data.fd = fd;

			if (epoll_ctl(MultiplexHandler::instance().getFd(), EPOLL_CTL_ADD, fd, &newEvent) < 0) {
				throw ; // TODO: implement
			}

#endif
		}
	}
}


MultiplexHandler::eventList		MultiplexHandler::eventHandler(const MultiplexHandler::eventTimer* timeout) {
	eventList	res;
	int			eventCount(0);
	SocketEvent::s_event		newEvents[1024];
#ifdef __DARWIN__

	eventCount = kevent(MultiplexHandler::instance().getFd(), &m_ChangeList[0], m_ChangeList.size(), &newEvents[0], MultiplexHandler::instance().getMaxEvent(), timeout);
	m_ChangeList.clear();

#endif
#ifdef __LINUX__

	eventCount = epoll_wait(MultiplexHandler::instance().getFd(), newEvent, MultiplexHandler::instance().getMaxEvent(), *timeout);

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
