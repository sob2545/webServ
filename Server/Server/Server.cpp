#include "Server.hpp"

Server::Server(const CONF::ServerBlock& block)
 : m_Socket(block.getIP(), block.getPort())
{
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

bool	Server::findServerBlock(const std::string& IP, const unsigned short& port) {
	for (ConfServerVector::const_iterator it = m_ServerBlock.begin(); it != m_ServerBlock.end(); ++it) {
		if (IP == it->getIP() && port == it->getPort()) {
			return true;
		}
	}
	return false;
}
