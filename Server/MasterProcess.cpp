#include "MasterProcess.hpp"

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include <cstddef>
#include <iterator>

typedef std::vector<CONF::ServerBlock> confServerBlockVector;

MasterProcess::mainServerVector	MasterProcess::m_Servers;

// TODO: delete
#include <iostream>

MasterProcess::MasterProcess(const std::string& fileName, char** env) {
	try {
		CONF::ConfBlock::initInstance(fileName, env);
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterProcess destroyed\n";
	CONF::ConfBlock::getInstance()->destroy();
}

ft::shared_ptr<Server>	MasterProcess::findServer(const std::string& IP, const unsigned short& port) {
	for (std::size_t i(0); i < m_Servers.size(); ++i) {
		if (m_Servers[i]->findServerBlock(IP, port)) {
			return m_Servers[i];
		}
	}
	return (ft::shared_ptr<Server>());
}

void	MasterProcess::start() {
	CONF::ConfBlock::getInstance()->print();

	const confServerBlockVector&	mainServerBlocks = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getServerVector();
	unsigned short		curPort = 0;

	// TODO: 호스트네임이 각각의 port 블럭에서 중복되는 것이 있는지 확인,
	//	실제 nginx에서도 만약 포트는 다른데 호스트가 동일한 경우 작동되는지 확인 필요!
	for (std::size_t i(0); i < mainServerBlocks.size(); ++i) {
		const ft::shared_ptr<Server> tmp = findServer(mainServerBlocks[i].getIP(), mainServerBlocks[i].getPort());
		(tmp.get()) ? tmp->insertServerBlock(mainServerBlocks[i]) : MasterProcess::m_Servers.push_back(ft::shared_ptr<Server>(::new Server(mainServerBlocks[i])));
	}
}