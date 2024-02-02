#include "Server.hpp"

// TODO: delete
#include <iostream>

Server::Server(const CONF::ServerBlock& block)
 : m_Socket(block.getIP(), block.getPort())
{
	m_ServerBlock.push_back(block);
}

Server::~Server() {}

/**
 *		hostname을 검색하면 원하는 서버 블록이 나오고, 만약 없으면 첫 번째 서버 블록이 나옴
*/
const CONF::ServerBlock&	Server::operator[](const std::string& hostname) {
	for (ConfServerVector_t::const_iterator it = m_ServerBlock.begin(); it != m_ServerBlock.end(); ++it) {
		const std::set<std::string>&	serverList = it->getServerNames();
		if (serverList.find(hostname) != serverList.end()) {
			return *it;
		}
	}
	return (*m_ServerBlock.begin());
}

void	Server::insertServerBlock(const CONF::ServerBlock& serverBlock) {
	this->m_ServerBlock.push_back(serverBlock);
}

void	Server::checkDuplicateHost(const std::set<std::string> &serverNames) {
	for (std::set<std::string>::const_iterator confServerNames = serverNames.begin(); confServerNames != serverNames.end(); ++confServerNames) {
		for (ConfServerVector_t::const_iterator serverBlock = m_ServerBlock.begin(); serverBlock != m_ServerBlock.end(); ++serverBlock) {
			std::cout << BOLDCYAN << *confServerNames << "conf server name" << RESET << std::endl;
			const std::set<std::string>::const_iterator		existServerName = serverBlock->getServerNames().find(*confServerNames);
			if (existServerName != serverBlock->getServerNames().end() || existServerName->size() == confServerNames->size()) {
				// TODO: implement error log 
				std::cerr << BOLDRED << *existServerName << " is duplicated " << *confServerNames << RESET << std::endl;
			}
		}
	}
}

bool	Server::findSameConfServerBlock(const CONF::ServerBlock& serverBlock) {
	for (ConfServerVector_t::const_iterator it = m_ServerBlock.begin(); it != m_ServerBlock.end(); ++it) {
		if (serverBlock.getIP() == it->getIP() && serverBlock.getPort() == it->getPort()) {
			checkDuplicateHost(serverBlock.getServerNames());
			return true;
		}
	}
	return false;
}

const int&	Server::getServerFd() const {
	return this->m_Socket.getFd();
}