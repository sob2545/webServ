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


public:
	Server(const ft::shared_ptr<CONF::ServerBlock>& block);
	~Server();

};