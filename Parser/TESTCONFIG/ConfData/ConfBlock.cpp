#include "ConfBlock.hpp"
#include "ConfMainBlock.hpp"

// TODO: delete
#include <iostream>

CONF::ConfBlock*	CONF::ConfBlock::instance = NULL;

void	CONF::ConfBlock::initInstance(const std::string& file) {
	if (CONF::ConfBlock::instance == NULL) {
		CONF::ConfBlock::instance = new CONF::ConfBlock(file);
	}
}

CONF::ConfBlock*	CONF::ConfBlock::getInstance() {
	return CONF::ConfBlock::instance;
}

CONF::ConfBlock::ConfBlock(const std::string& file) {
	CONF::ConfFile::InitInstance(file);

	CONF::ConfBlock::m_MainBlock.initialize();
}

CONF::ConfBlock::~ConfBlock() {
}


const CONF::MainBlock&	CONF::ConfBlock::getHttpBlock() const {
	return CONF::ConfBlock::m_MainBlock;
}

// DEBUG
void	CONF::ConfBlock::print() {
	std::cout << "Main Block" << std::endl;
	std::cout << "\tEnv: " << std::endl;
	for (const auto& env : this->m_MainBlock.m_Env) {
		std::cout << "\t\t" << env.first << "=" << env.second << std::endl;
	}
	std::cout << "\tWorker_process: " << this->m_MainBlock.m_Worker_process << std::endl;
	std::cout << "\tDaemon: " << (this->m_MainBlock.m_Daemon? "on" : "off") << std::endl;
	std::cout << "\tTime_resolution: " << this->m_MainBlock.m_Timer_resolution << std::endl;
	std::cout << "\tWorker_connections: " << this->m_MainBlock.m_Event_block.m_Worker_connections << std::endl;
	for (const auto& log : this->m_MainBlock.m_Error_log) {
		std::cout << "\tError_log: " << log << std::endl;
	}
 }