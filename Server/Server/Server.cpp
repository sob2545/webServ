#include "Server.hpp"

// TODO: delete
#include <iostream>

Server::Server(const CONF::ServerBlock& block)
 : m_Socket(block.getIP(), block.getPort())
{
	m_ServerBlock.push_back(block);
	// 생성자에서 소켓을 바로 생성하면 안됨. 포트를 기준으로 생성할지 넘어갈지 결정해야함.
	// 하나의 포트에 여러 개의 서버를 받을 수 있도록 하여, shared_ptr을 사용해 공유하도록 함.
}

Server::~Server() {}

/**
 *		hostname을 검색하면 원하는 서버 블록이 나오고, 만약 없으면 첫 번째 서버 블록이 나옴
*/
const CONF::ServerBlock&	Server::operator[](const std::string& hostname) {
	for (ConfServerVector::const_iterator it = m_ServerBlock.begin(); it != m_ServerBlock.end(); ++it) {
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
	for (std::set<std::string>::const_iterator it = serverNames.begin(); it != serverNames.end(); ++it) {
		for (ConfServerVector::const_iterator it2 = m_ServerBlock.begin(); it2 != m_ServerBlock.end(); ++it2) {
			if (it2->getServerNames().find(*it) != it2->getServerNames().end()) {
				// TODO: error log 또는 terminal에 출력
				std::cerr << *it <<  " Error: duplicate host name\n";
			}
		}
	}
}

bool	Server::findSameConfServerBlock(const std::string& IP, const unsigned short& port) {
	for (ConfServerVector::const_iterator it = m_ServerBlock.begin(); it != m_ServerBlock.end(); ++it) {
		if (IP == it->getIP() && port == it->getPort()) {
			checkDuplicateHost(it->getServerNames());
			return true;
		}
	}
	return false;
}
