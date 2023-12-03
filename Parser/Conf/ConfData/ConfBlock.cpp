#include "ConfBlock.hpp"
#include "../ConfParser/ConfParser.hpp"

// TODO: delete
#include <iostream>

CONF::ConfBlock*	CONF::ConfBlock::instance = NULL;

CONF::ConfBlock*	CONF::ConfBlock::initInstance(const std::string& file) {
	if (CONF::ConfBlock::instance == NULL) {
		CONF::ConfBlock::instance = new CONF::ConfBlock(file);
	}
	return CONF::ConfBlock::instance;
}

CONF::ConfBlock*	CONF::ConfBlock::getInstance() {
	return CONF::ConfBlock::instance;
}

CONF::ConfBlock::ConfBlock(const std::string& file) {
	ConfParser	parser(file);
	parser.Parsing(this->m_MainBlock);
}

CONF::ConfBlock::~ConfBlock() {
}

const bool	CONF::ConfBlock::isDaemonOn() {
	return this->m_MainBlock.m_Daemon;
}

const unsigned int&	CONF::ConfBlock::getWorkerProcess() {
	return this->m_MainBlock.m_Worker_process;
}

const unsigned long&	CONF::ConfBlock::getTimeResolution() {
	return this->m_MainBlock.m_Timer_resolution;
}

const CONF::ConfBlock::errorLogLocation&	CONF::ConfBlock::getErrorLog() {
	return this->m_MainBlock.m_Error_log;
}

const CONF::ConfBlock::envMap&	CONF::ConfBlock::getEnvMap() {
	return this->m_MainBlock.m_Env;
}

const std::string&	CONF::ConfBlock::getEnv(const std::string& key) {
	return this->m_MainBlock.m_Env[key];
}

const unsigned int&	CONF::ConfBlock::getWorkerConnections() {
	return this->m_MainBlock.m_Event_block.m_Worker_connections;
}

// const CONF::httpBlock&	CONF::ConfBlock::getHttpBlock() {


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