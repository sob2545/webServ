#include "ConfBlock.hpp"
#include "../Parser/Exception/ConfParserException.hpp"
#include "ConfMainBlock.hpp"
#include "../EnvParser/EnvParser.hpp"

// TODO: delete
#include <iostream>

CONF::ConfBlock*	CONF::ConfBlock::instance = NULL;

CONF::MainBlock		CONF::ConfBlock::m_MainBlock;

void	CONF::ConfBlock::initInstance(const std::string& file, char** env) {
	if (CONF::ConfBlock::instance == NULL) {
		CONF::ConfBlock::instance = new CONF::ConfBlock(file, env);
	}
}

CONF::ConfBlock*	CONF::ConfBlock::getInstance() {
	if (CONF::ConfBlock::instance == NULL) {
		throw ConfParserException("ConfBlock::getInstance()", "instance is NULL");
	}
	return CONF::ConfBlock::instance;
}

CONF::ConfBlock::ConfBlock(const std::string& file, char** env) {
	CONF::ConfFile::InitInstance(file);
	ENV::EnvParser<EnvParserException>(this->m_MainBlock.setEnvMap(), env);

	CONF::ConfBlock::m_MainBlock.initialize();
}

CONF::ConfBlock::~ConfBlock() {
}


const CONF::MainBlock&	CONF::ConfBlock::getMainBlock() const {
	return CONF::ConfBlock::m_MainBlock;
}

// DEBUG
void	CONF::ConfBlock::print() {
	std::cout << "Main Block" << std::endl;
	std::cout << "\tEnv: " << std::endl;
	for (auto it = this->m_MainBlock.getEnvMap().begin(); it != this->m_MainBlock.getEnvMap().end(); ++it) {
		std::cout << "\t" << it->first << " " << it->second;
	}
	std::cout << "\tWorker_process: " << this->m_MainBlock.getWorkerProcess() << std::endl;
	std::cout << "\tDaemon: " << (this->m_MainBlock.isDaemonOn()? "on" : "off") << std::endl;
	std::cout << "\tTime_resolution: " << this->m_MainBlock.getTimeResolution() << std::endl;
		std::cout << "\tError_log: " << this->m_MainBlock.getErrorLog() << std::endl;
 }