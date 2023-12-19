#pragma once

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"
#include "../Utils/Singleton.hpp"
#include "Server/Server.hpp"

class MasterProcess : public Singleton<MasterProcess> {
public:
	typedef std::vector<ft::shared_ptr<Server> >	mainServerVector;

private:
	static mainServerVector	m_Servers;

	MasterProcess(const MasterProcess& other);
	MasterProcess& operator=(const MasterProcess& other);

private:
	static ft::shared_ptr<Server>	findServer(const std::string& IP, const unsigned short& port);

public:
	MasterProcess(const std::string& fileName, char** env);
	~MasterProcess();


	static void	start();
};