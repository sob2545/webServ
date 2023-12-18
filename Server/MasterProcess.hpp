#pragma once

#include "../Parser/ConfParser/Data/ConfBlock.hpp"
#include "../Utils/Singleton.hpp"
#include "Server/Server.hpp"

class MasterProcess : public Singleton<MasterProcess> {
public: typedef std::map<std::vector<std::string>, ft::shared_ptr<Server> > serverMap;
private:
	// static serverMap	m_Servers;


	MasterProcess(const MasterProcess& other);
	MasterProcess& operator=(const MasterProcess& other);

public:
	MasterProcess(const std::string& fileName, char** env);
	~MasterProcess();

	static void	start();
};