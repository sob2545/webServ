#pragma once

#include "../../FileDescriptor/Socket/ServerSocket.hpp"
#include "../../Parser/ConfParser/ConfData/ConfServerBlock.hpp"

class Server {
private:
	ft::shared_ptr<CONF::ServerBlock>	m_ServerBlock;
	ServerSocket						m_Socket;

	Server(const Server& other);
	Server& operator=(const Server& other);

private:
	unsigned short	convertIPv4Address(const std::string& IP);


public:
	Server(const ft::shared_ptr<CONF::ServerBlock>& block);
	~Server();

};