#include "MasterProcess.hpp"

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "../Multiplexing/MultiplexHandler.hpp"
#include "WorkerProcess/WorkerProcess.hpp"
#include <sys/_types/_pid_t.h>

typedef std::vector<CONF::ServerBlock> confServerBlockVector_t;

MasterProcess::MainServerVector_t	MasterProcess::m_Servers;

MasterProcess::MIMEMap_t	MasterProcess::m_MIMETypes;

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

bool	MasterProcess::isServerSocket(const int& fd) {
	for (MainServerVector_t::const_iterator it = m_Servers.begin(); it != m_Servers.end(); ++it) {
		if (it->get()->getServerFd() == fd) {
			
		}
	}
}



void	MasterProcess::runWorkerProcess() {
	while (1) {
		const std::vector<SocketEvent> eventList = MultiplexHandler::eventHandler(NULL);
		for (std::size_t eventNumber; eventNumber < eventList.size(); ++eventNumber) {
			const SocketEvent&	currEvent = eventList[eventNumber];

			if (currEvent.isReadEvent()) {


			} else if (currEvent.isWriteEvent()) {

			}
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

	const confServerBlockVector_t&	mainServerBlocks = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getServerVector();
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
				for (MainServerVector_t::const_iterator it = MasterProcess::m_Servers.begin(); it != MasterProcess::m_Servers.end(); ++it) {
					MultiplexHandler::addServerEvent(it->get()->getServerFd());
				}

				/* main server loop */
				runWorkerProcess();

#ifdef __RELEASE__

			}
		}
	}

#endif
}

const MasterProcess::MIMEMap_t&	MasterProcess::getMIMETypes() {
	return m_MIMETypes;
}