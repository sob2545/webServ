#pragma once

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "../Utils/Singleton.hpp"
#include "Server/Server.hpp"
#include "Client/Client.hpp"

class MasterProcess : public Singleton<MasterProcess> {
public:
	typedef std::vector<ft::shared_ptr<Server> >				MainServerVector_t;
	typedef std::map<std::string, std::vector<std::string> >	MIMEMap_t;
	typedef std::map<int, ft::unique_ptr<Client> >				ClientMap_t;

private:
	static	MainServerVector_t		m_Servers;
	static	MIMEMap_t				m_MIMETypes;

	MasterProcess(const MasterProcess& other);
	MasterProcess& operator=(const MasterProcess& other);

private:
	static ft::shared_ptr<Server>	findExistServer(const std::string& IP, const unsigned short& port);
	static void						runWorkerProcess();
	static bool						isServerSocket(const int& fd);

public:
	MasterProcess(const std::string& fileName, char** env);
	~MasterProcess();

	static void	start();
	static const MIMEMap_t&	getMIMETypes();
};