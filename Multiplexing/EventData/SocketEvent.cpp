#include "SocketEvent.hpp"
#include <cstring>
#include <sys/event.h>

SocketEvent::SocketEvent()
: m_Status(0)
{
	std::memset(&m_Event, 0, sizeof(m_Event));
}

SocketEvent::SocketEvent(const s_event& other) {
	#ifdef	__DARWIN__

	this->m_Event.ident = other.ident;
	this->m_Event.filter = other.filter;
	this->m_Event.flags = other.flags;
	this->m_Event.fflags = other.fflags;
	this->m_Event.data = other.data;
	// void* 넘겨줄 때 조심해야됨 (shared_ptr 등 사용)
	this->m_Event.udata = other.udata;

	(this->m_Event.filter & EVFILT_READ) ? m_Status |= E_EV::READ : 0;
	(this->m_Event.filter & EVFILT_WRITE) ? m_Status |= E_EV::WRITE : 0;

#endif
#ifdef __LINUX__

	this->m_Event.events = other.events;
	this->m_Event.data.fd = other.data.fd;

	(this->m_Event.events & EPOLLIN) ? m_Status |= E_EV::READ : 0;
	(this->m_Event.events & EPOLLOUT) ? m_Status |= E_EV::WRITE : 0;

#endif
}

SocketEvent::SocketEvent(const SocketEvent& other)
: m_Status(other.m_Status),
  m_Event(other.m_Event)
{}

const SocketEvent&	SocketEvent::operator=(const SocketEvent& other) {
	this->m_Status = other.m_Status;
	this->m_Event = other.m_Event;
	return *this;
}

// uintptr_t ident, int16_t filter, uint16_t flags, uint32_t fflags, int64_t data, void *udata
const SocketEvent&	SocketEvent::operator=(const s_event& other) {
#ifdef	__DARWIN__

	this->m_Event.ident = other.ident;
	this->m_Event.filter = other.filter;
	this->m_Event.flags = other.flags;
	this->m_Event.fflags = other.fflags;
	this->m_Event.data = other.data;
	// void* 넘겨줄 때 조심해야됨 (shared_ptr 등 사용)
	this->m_Event.udata = other.udata;

#endif
#ifdef __LINUX__

	this->m_Event.events = other.events;
	this->m_Event.data.fd = other.data.fd;

#endif
	return *this;
}

SocketEvent::~SocketEvent() {}

const int	SocketEvent::getFd() const {
#ifdef __DARWIN__

	return (this->m_Event.ident);

#endif
#ifdef __LINUX__

	return (this->m_Event.data.fd);

#endif
}

const short&	SocketEvent::getStatus() const {
	return this->m_Status;
}