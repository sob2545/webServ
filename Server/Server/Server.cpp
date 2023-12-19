#include "Server.hpp"

Server::Server(const ft::shared_ptr<CONF::ServerBlock>& block)
  : m_ServerBlock(block),
    m_Socket(block->getPort(), block->getIP())
{
	// 생성자에서 소켓을 바로 생성하면 안됨. 포트를 기준으로 생성할지 넘어갈지 결정해야함.
	// 하나의 포트에 여러 개의 서버를 받을 수 있도록 하여, shared_ptr을 사용해 공유하도록 함.
}

Server::~Server() {

}


