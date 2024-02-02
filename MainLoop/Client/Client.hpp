#pragma once

#include "./ClientData/RequestRecipe.hpp"
#include "../../FileDescriptor/Socket/ClientSocket.hpp"

class Client {
private:
	int						m_ServerFd;
	bool					m_CloseStatus;
	ClientSocket			m_Socket;
	std::string				m_ReadBuffer;
	std::string				m_WriteBuffer;
	HTTP::RequestRecipe		m_Data;


private:
	Client(const Client& other);
	const Client&	operator=(const Client& other);

public:
	Client(const int& serverFd);
	~Client();

	const int&				getFd() const;
	const std::string&		getReadBuffer() const;
	void					setReadBuffer(const std::string& data);
	const std::string&		getWriteBuffer() const;
	void					setWriteBuffer(const std::string& data);
	const bool&				getCloseStatus() const;
	void					setCloseStatus();
	HTTP::RequestRecipe&	getRequestRecipe();
};
