#pragma once

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "WorkerEventHandler/WorkerEventHandler.hpp"
#include "../Utils/Semaphore.hpp"

class MasterProcess : public Singleton<MasterProcess> {
public:
	typedef std::map<int, ft::shared_ptr<Server> >				ServerMap_t;
	typedef std::map<int, ft::shared_ptr<Client> >				ClientMap_t;
	typedef std::map<std::string, std::vector<std::string> >	MIMEMap_t;

private:
	static ServerMap_t		m_Servers;
	static ClientMap_t		m_Clients;
	static MIMEMap_t		m_MIMETypes;

	MasterProcess(const MasterProcess& other);
	MasterProcess& operator=(const MasterProcess& other);

private:
	static ft::shared_ptr<Server>	findExistServer(const std::string& IP, const unsigned short& port);
	static void						runWorkerEventHandler();
	static bool						isServerSocket(const int& fd);
	static void						CheckClientConnection(const int& currFd);

public:
	MasterProcess(const std::string& fileName, char** env);
	~MasterProcess();

	static void	start();
	static const MIMEMap_t&	getMIMETypes();
};