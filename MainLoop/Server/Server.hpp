#pragma once

#include "../../FileDescriptor/Socket/ServerSocket.hpp"
#include "../../Parser/ConfParser/ConfData/ConfServerBlock.hpp"

class Server {
public:
	typedef std::vector<CONF::ServerBlock> ConfServerVector_t;
private:
	ServerSocket		m_Socket;
	ConfServerVector_t	m_ServerBlock;

	Server(const Server& other);
	Server& operator=(const Server& other);

private:


public:
	Server(const CONF::ServerBlock& block);
	~Server();

	const CONF::ServerBlock&	operator[](const std::string& hostname);

	void		insertServerBlock(const CONF::ServerBlock& serverBlock);
	void		checkDuplicateHost(const std::set<std::string>& serverNames);
	bool		findSameConfServerBlock(const CONF::ServerBlock& serverBlock);

	const int&	getServerFd() const;

};