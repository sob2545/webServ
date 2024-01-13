#include "MasterProcess.hpp"

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "../Multiplexing/MultiplexHandler.hpp"
#include <sys/_types/_pid_t.h>

typedef std::vector<CONF::ServerBlock> confServerBlockVector;

MasterProcess::mainServerVector	MasterProcess::m_Servers;

MasterProcess::TypeMap	MasterProcess::m_MIMETypes;

// TODO: delete
#include <iostream>

MasterProcess::MasterProcess(const std::string& fileName, char** env) {
	try {
		CONF::ConfBlock::initInstance(fileName, env);
	} catch (ConfParserException& e) {
		throw (e);
	}
	m_MIMETypes = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getMime_types();
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterProcess destroyed\n";
	CONF::ConfBlock::getInstance()->destroy();
}

ft::shared_ptr<Server>	MasterProcess::findExistServer(const std::string& IP, const unsigned short& port) {
	for (std::size_t i(0); i < m_Servers.size(); ++i) {
		if (m_Servers[i]->findSameConfServerBlock(IP, port)) {
			return m_Servers[i];
		}
	}
	return (ft::shared_ptr<Server>());
}

void	MasterProcess::workerProcessRun() {
	while (1) {
		const std::vector<SocketEvent>	eventList = MultiplexHandler::eventHandler(NULL);
		for (int eventNum(0); eventNum < eventList.size(); ++eventNum) {
			const SocketEvent* const	curEvent = &eventList[eventNum];
			
		}
	}
}

void	MasterProcess::start() {
	CONF::ConfBlock::getInstance()->print();

	/*
	 *	Daemon mode
	*/
#ifdef __RELEASE__

	if (CONF::ConfBlock::getMainBlock::isDaemonOn()) {
		pid_t	curPid = fork();
		switch (curPid) {
			case (0) : {
				std::cout << BOLDGREEN << "webServ running in background mode" << RESET << std::endl;
				exit(0);
			}
		}
	}

#endif

	const confServerBlockVector&	mainServerBlocks = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getServerVector();
	unsigned short		curPort = 0;

	for (std::size_t i(0); i < mainServerBlocks.size(); ++i) {
		const ft::shared_ptr<Server>	tmp = findExistServer(mainServerBlocks[i].getIP(), mainServerBlocks[i].getPort());

		try {
			(tmp.get()) ? tmp->insertServerBlock(mainServerBlocks[i]) : MasterProcess::m_Servers.push_back(ft::shared_ptr<Server>(::new Server(mainServerBlocks[i])));
		} catch (SOCK::SocketException& e) {
			// TODO: error log 또는 terminal에 출력
			std::cerr << e.getMessage() << std::endl;
		} catch (std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}

	if (m_Servers.empty()) {
		// TODO: 서버 블록이 하나도 없을 때 예외처리
		throw (std::runtime_error("Error: no server block"));
	}

#ifdef __RELEASE__

	const int&	workerProcessNumber = CONF::ConfBlock::getInstance()->getMainBlock().getWorkerProcess();
	for (int worker(0); worker < workerProcessNumber; ++worker) {
		pid_t	workerFd = fork();

		switch (workerFd) {
			case (0): {

#endif

				/* kqueue | epoll init */
				MultiplexHandler::instance();

				/* add Server event */
				for (mainServerVector::const_iterator it = MasterProcess::m_Servers.begin(); it != MasterProcess::m_Servers.end(); ++it) {
					MultiplexHandler::addServerEvent(it->get()->getServerFd());
				}

				/* main server loop */
				workerProcessRun();

#ifdef __RELEASE__

			}
		}
	}

#endif
}

const MasterProcess::TypeMap&	MasterProcess::getMIMETypes() {
	return m_MIMETypes;
}