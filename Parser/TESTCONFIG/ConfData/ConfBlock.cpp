#include "ConfBlock.hpp"
#include "ConfMainBlock.hpp"
#include "../ConfParser/Exception/ConfParserException.hpp"

// TODO: delete
#include <iostream>

CONF::ConfBlock*	CONF::ConfBlock::instance = NULL;

CONF::MainBlock		CONF::ConfBlock::m_MainBlock;

void	CONF::ConfBlock::initInstance(const std::string& file) {
	if (CONF::ConfBlock::instance == NULL) {
		CONF::ConfBlock::instance = new CONF::ConfBlock(file);
	}
}

CONF::ConfBlock*	CONF::ConfBlock::getInstance() {
	if (CONF::ConfBlock::instance == NULL) {
		throw ConfParserException("ConfBlock.cpp", "ConfBlock::getInstance()", "instance is NULL", CONF::ConfFile::getInstance()->Pos());
	}
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
	std::cout << "\tWorker_process: " << this->m_MainBlock.getWorkerProcess() << std::endl;
	std::cout << "\tDaemon: " << (this->m_MainBlock.isDaemonOn()? "on" : "off") << std::endl;
	std::cout << "\tTime_resolution: " << this->m_MainBlock.getTimeResolution() << std::endl;
	for (const auto& log : this->m_MainBlock.getErrorLog()) {
		std::cout << "\tError_log: " << log << std::endl;
	}
 }