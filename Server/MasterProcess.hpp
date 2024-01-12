#pragma once

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "../Utils/Singleton.hpp"
#include "Server/Server.hpp"

class MasterProcess : public Singleton<MasterProcess> {
public:
	typedef std::vector<ft::shared_ptr<Server> >				mainServerVector;
	typedef std::map<std::string, std::vector<std::string> >	TypeMap;

private:
	static	mainServerVector	m_Servers;
	static	TypeMap				m_MIMETypes;

	MasterProcess(const MasterProcess& other);
	MasterProcess& operator=(const MasterProcess& other);

private:
	static ft::shared_ptr<Server>	findExistServer(const std::string& IP, const unsigned short& port);
	static void						workerProcessRun();

public:
	MasterProcess(const std::string& fileName, char** env);
	~MasterProcess();

	static void	start();
	static const TypeMap&	getMIMETypes();
};