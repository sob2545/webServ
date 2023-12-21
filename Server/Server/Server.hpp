#pragma once

#include "../../FileDescriptor/Socket/ServerSocket.hpp"
#include "../../Parser/ConfParser/ConfData/ConfServerBlock.hpp"

class Server {
public:
	typedef std::vector<CONF::ServerBlock> ConfServerVector;
private:
	ServerSocket		m_Socket;
	ConfServerVector	m_ServerBlock;

	Server(const Server& other);
	Server& operator=(const Server& other);

private:


public:
	Server(const CONF::ServerBlock& block);
	~Server();

	const CONF::ServerBlock&	operator[](const std::string& hostname);

	void	insertServerBlock(const CONF::ServerBlock& serverBlock);
	bool	findSameConfServerBlock(const std::string& IP, const unsigned short& port);
	void	checkDuplicateHost(const std::set<std::string>& serverNames);

};