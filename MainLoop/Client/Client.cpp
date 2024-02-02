#include "Client.hpp"
#include "ClientData/RequestRecipe.hpp"

Client::Client(const int& serverFd)
 : m_ServerFd(serverFd),
   m_CloseStatus(false),
   m_Socket()
{
#if defined (__RELEASE__)
	m_Socket.accept(serverFd) ? 0 : throw SemLockFail();
#else
	m_Socket.accept(serverFd);
#endif
}

Client::~Client() {}

const int&	Client::getFd() const {
	return (m_Socket.getFd());
}

const std::string&	Client::getReadBuffer() const {
	return (this->m_ReadBuffer);
}

void	Client::setReadBuffer(const std::string& data) {
	this->m_ReadBuffer = data;
}

const std::string&	Client::getWriteBuffer() const {
	return (this->m_WriteBuffer);
}

void	Client::setWriteBuffer(const std::string& data) {
	this->m_WriteBuffer = data;
}

HTTP::RequestRecipe&	 Client::getRequestRecipe() {
	return (this->m_Data);
}

const bool&	Client::getCloseStatus() const {
	return this->m_CloseStatus;
}

void	Client::setCloseStatus() {
	this->m_CloseStatus = true;
}