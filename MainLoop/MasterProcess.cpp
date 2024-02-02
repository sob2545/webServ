#include "MasterProcess.hpp"

#include "../Multiplexing/MultiplexHandler.hpp"
#include "Client/Client.hpp"
#include "WorkerEventHandler/Exception/ClientCloseException.hpp"
#include "WorkerEventHandler/WorkerEventHandler.hpp"
#include <utility>

typedef std::vector<CONF::ServerBlock> confServerBlockVector_t;

MasterProcess::ServerMap_t	MasterProcess::m_Servers;
MasterProcess::ClientMap_t	MasterProcess::m_Clients;

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

ft::shared_ptr<Server>	MasterProcess::findExistServer(const CONF::ServerBlock& serverBlock) {
	for (ServerMap_t::const_iterator it = m_Servers.begin(); it != m_Servers.end(); ++it) {
		if (it->second->findSameConfServerBlock(serverBlock)) {
			return it->second;
		}
	}
	return (ft::shared_ptr<Server>());
}

bool	MasterProcess::isServerSocket(const int& fd) {
	for (ServerMap_t::const_iterator it = m_Servers.begin(); it != m_Servers.end(); ++it) {
		if (it->first == fd) {
			return (true);
		}
	}
	return (false);
}

void	MasterProcess::CheckClientConnection(const int& currFd) {
	if (m_Clients[currFd]->getCloseStatus()) {
		std::cout << BOLDRED << "client closed\n" << RESET;
		close(currFd);
		m_Clients.erase(currFd);
	} else {
		MultiplexHandler::addClientEvent(currFd, E_EV::READ);
	}
}


void	MasterProcess::runWorkerEventHandler() {
#ifdef DEBUG
#endif

	while (1) {
		const std::vector<SocketEvent> eventList = MultiplexHandler::eventHandler(NULL);

		for (std::size_t eventNumber(0); eventNumber < eventList.size(); ++eventNumber) {
			const SocketEvent&	currEvent = eventList[eventNumber];
			const int&			currFd = currEvent.getFd();
			ft::shared_ptr<Client>&	currClient = m_Clients.find(currFd)->second;

			if (currEvent.isReadEvent()) {
				if (isServerSocket(currFd)) {
					ft::shared_ptr<Client>	newClient = WorkerEventHandler::makeClient(currFd);
					m_Clients.insert(std::make_pair(newClient->getFd(), newClient));
				} else {
					try {
						WorkerEventHandler::recvFromClient(currClient);
					} catch (ClientCloseException& e) {
						m_Clients.erase(currClient->getFd());
						continue ;
					}
					WorkerEventHandler::parseRequest(currClient);
				}
			} else if (currEvent.isWriteEvent()) {
				WorkerEventHandler::makeResponse(currClient);
				WorkerEventHandler::sendToClient(currClient);
				CheckClientConnection(currClient->getFd());
			}
		}
	}
}

void	MasterProcess::start() {
	// CONF::ConfBlock::getInstance()->print();

	/*
	 *	Daemon mode
	*/
#ifdef __RELEASE__

	if (CONF::ConfBlock::getMainBlock::isDaemonOn()) {
		pid_t	currPid = fork();
		switch (currPid) {
			case (0) : {
				std::cout << BOLDGREEN << "webServ running in background mode" << RESET << std::endl;
				exit(0);
			}
		}
	}

#endif

	const confServerBlockVector_t&	confServerBlockVector = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getServerVector();
	unsigned short		curPort = 0;

	for (std::size_t i(0); i < confServerBlockVector.size(); ++i) {
		const ft::shared_ptr<Server>	tmp = findExistServer(confServerBlockVector[i]);

		
		try {
			if (tmp.get()) {
				m_Servers.find(tmp->getServerFd())->second->insertServerBlock(confServerBlockVector[i]);
			} else {
				ft::shared_ptr<Server>	newServer(::new Server(confServerBlockVector[i]));
				m_Servers.insert(std::make_pair(newServer->getServerFd(), newServer));
			}
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
	int	semCount(0);

	ft::Semaphore::instance();
	for (ServerMap_t::const_iterator serverIt = m_Servers.begin(); serverIt != m_Servers.end(); ++serverIt, ++semCount) {
		ft::Semaphore::setSemaphores(semCount, serverIt->first);
	}


	const int&	workerProcessNumber = CONF::ConfBlock::getInstance()->getMainBlock().getWorkerProcess();
	for (int worker(0); worker < workerProcessNumber; ++worker) {
		pid_t	workerFd = fork();

		switch (workerFd) {
			case (0): {

#endif

				/* kqueue | epoll init */
				MultiplexHandler::instance();

				/* add Server event */
				for (ServerMap_t::const_iterator it = MasterProcess::m_Servers.begin(); it != MasterProcess::m_Servers.end(); ++it) {
					MultiplexHandler::addServerEvent(it->first);
				}
				/* main server loop */
				runWorkerEventHandler();

#ifdef __RELEASE__

			}
		}
	}

#endif
}

const MasterProcess::MIMEMap_t&	MasterProcess::getMIMETypes() {
	return m_MIMETypes;
}