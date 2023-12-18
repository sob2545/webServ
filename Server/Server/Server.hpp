#pragma once

#include "../../FileDescriptor/Socket/ServerSocket.hpp"
#include "../../Parser/ConfParser/Data/ConfServerBlock.hpp"

class Server {
private:
	ft::shared_ptr<CONF::ServerBlock>	m_ServerBlock;
	ServerSocket						m_Socket;

	Server(const Server& other);
	Server& operator=(const Server& other);

public:
	Server(const ft::shared_ptr<CONF::ServerBlock>& block);
	~Server();

};