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
	/*
	for (auto it = this->m_MainBlock.getEnvMap().begin(); it != this->m_MainBlock.getEnvMap().end(); ++it) {
		std::cout << "\t" << it->first << " " << it->second << std::endl;
	}
	*/
	std::cout << "\tWorker_process: " << this->m_MainBlock.getWorkerProcess() << std::endl;
	std::cout << "\tDaemon: " << (this->m_MainBlock.isDaemonOn()? "on" : "off") << std::endl;
	std::cout << "\tTime_resolution: " << this->m_MainBlock.getTimeResolution() << std::endl;
		std::cout << "\tError_log: " << this->m_MainBlock.getErrorLog() << std::endl;
	
	std::cout << "HTTP Block" << std::endl;
	for (auto it = this->m_MainBlock.getHTTPBlock().getMime_types().begin(); it != this->m_MainBlock.getHTTPBlock().getMime_types().end(); ++it) {
		std::cout << "\t" << it->first << " ";
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) {
			std::cout << *it2 << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "\t========" << std::endl;
	std::cout << "\tDefault_type: " << this->m_MainBlock.getHTTPBlock().getDefault_type() << std::endl;
	std::cout << "\tAccess log: " << this->m_MainBlock.getHTTPBlock().getAccess_log() << std::endl;
	std::cout << "\tRoot: " << this->m_MainBlock.getHTTPBlock().getRoot() << std::endl;
	std::cout << "\tAutoindex: " << (this->m_MainBlock.getHTTPBlock().getAutoindex()? "on" : "off") << std::endl;
	std::cout << "\tIndex: " << this->m_MainBlock.getHTTPBlock().getIndex("index.html") << std::endl;

 }