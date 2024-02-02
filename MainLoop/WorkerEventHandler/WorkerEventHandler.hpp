#pragma once

#include "../../Utils/Singleton.hpp"
#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../../Multiplexing/MultiplexHandler.hpp"
#include "../../Parser/HTTPMessageParser/RequestMessageParser.hpp"
#include <sys/stat.h>

#define	MAX_READSIZE 4096

class WorkerEventHandler : public Singleton<WorkerEventHandler> {
private:
	WorkerEventHandler(const WorkerEventHandler& other);
	WorkerEventHandler& operator=(const WorkerEventHandler& other);

public:
	WorkerEventHandler();
	~WorkerEventHandler();

	static ft::shared_ptr<Client>	makeClient(const int& serverFd);
	static void						recvFromClient(ft::shared_ptr<Client>& currClient);
	static void						parseRequest(ft::shared_ptr<Client>& currClient);

	static void						makeResponse(ft::shared_ptr<Client>& currClient);
	static void						sendToClient(ft::shared_ptr<Client>& currClient);
	
};