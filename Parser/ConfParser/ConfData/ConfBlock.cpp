#include "ConfBlock.hpp"
#include "../AConfParser/Exception/ConfParserException.hpp"
#include "ConfMainBlock.hpp"
#include "../EnvParser/EnvParser.hpp"
#include "ConfServerBlock.hpp"
#include "errorPageData/errorPageData.hpp"

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
	try {
		CONF::ConfFile::InitInstance(file);
	} catch(FileException& e) {
		// TODO: implement
	}
	ENV::EnvParser<EnvParserException>(this->m_MainBlock.setEnvMap(), env);

	CONF::ConfBlock::m_MainBlock.initialize();
}

CONF::ConfBlock::~ConfBlock() {
}

void	CONF::ConfBlock::destroy() {
	delete CONF::ConfBlock::instance;
	CONF::ConfBlock::instance = NULL;
}

const CONF::MainBlock&	CONF::ConfBlock::getMainBlock() const {
	return CONF::ConfBlock::m_MainBlock;
}

// DEBUG
void	CONF::ConfBlock::print() {
	std::cout << "Main Block" << std::endl;
	std::cout << "\tEnv: " << std::endl;
	for (auto it = this->m_MainBlock.getEnvMap().begin(); it != this->m_MainBlock.getEnvMap().end(); ++it) {
		std::cout << "\t" << it->first << " " << it->second << std::endl;
	}
	std::cout << "===================================\n";
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
	std::cout << "\tIndex: " << this->m_MainBlock.getHTTPBlock().getIndex("index.htmllll") << std::endl;

	std::cout << "\t==================\n";
	for (auto it = this->m_MainBlock.getHTTPBlock().getError_page().begin(); it != this->m_MainBlock.getHTTPBlock().getError_page().end(); ++it) {
		std::cout << it->first << ": " << (int)it->second.m_Type << " " << it->second.m_Path << std::endl;
		std::cout << ((it->second.m_Type == E_ERRORPAGE::REPLACE) ? it->second.m_Replace : 0) << std::endl;
	}

	std::cout << BOLDGREEN << "\t========== Server Block ==============\n" << RESET;

	const std::vector<CONF::ServerBlock>& tmpServerMap = this->m_MainBlock.getHTTPBlock().getServerVector();
	for (auto it = tmpServerMap.begin(); it != tmpServerMap.end(); ++it) {
		// if (it->second) {
			std::cout << "\t\tAccess log: " << it->getAccess_log() << std::endl;
			std::cout << "\t\tRoot: " << it->getRoot() << std::endl;
			std::cout << "\t\tAutoindex: " << (it->getAutoindex() ? "on" : "off") << std::endl;
			std::cout << "\t\tIndex: " << it->getIndex("domain1.com") << std::endl;
			for (auto ser_it = it->getError_page().begin(); ser_it != it->getError_page().end(); ++ser_it) {
				std::cout << "\t\t\t" << ser_it->first << ": " << (int)ser_it->second.m_Type << " " << ser_it->second.m_Path << std::endl;
			std::cout << "\t\t\t" << ((ser_it->second.m_Type == E_ERRORPAGE::REPLACE) ? ser_it->second.m_Replace : 0) << std::endl;
			}
			std::cout << "\n";
			std::cout << "\t\tIP: " << it->getIP() << std::endl;
			std::cout << "\t\tPort: " << it->getPort() << std::endl;
			std::cout << "\n";
			std::cout << BOLDWHITE << "\t\tServer Names: " << RESET << std::endl;
			for (auto nam_it = it->getServerNames().begin(); nam_it != it->getServerNames().end(); ++nam_it) {
				std::cout << "\t\t\t" << *nam_it << std::endl;
			}
		// }

		std::cout << BOLDBLUE << "\t\t=================Location Block=================\n" << RESET;
		const std::map<std::string, LocationBlock>& tmpLocationMap_t = it->getLocationMap();
		for (auto it = tmpLocationMap_t.begin(); it != tmpLocationMap_t.end(); ++it) {
			std::cout << "\t\t\tLocation Name: " << it->first << std::endl;
			std::cout << "\t\t\t\tAccess log: " << it->second.getAccess_log() << std::endl;
			std::cout << "\t\t\t\tRoot: " << it->second.getRoot() << std::endl;
			std::cout << "\t\t\t\tAutoindex: " << (it->second.getAutoindex() ? "on" : "off") << std::endl;
			std::cout << "\t\t\t\tIndex: " << it->second.getIndex("domain1.com") << std::endl;
			for (auto ser_it = it->second.getError_page().begin(); ser_it != it->second.getError_page().end(); ++ser_it) {
				std::cout << "\t\t\t" << ser_it->first << ": " << (int)ser_it->second.m_Type << " " << ser_it->second.m_Path << std::endl;
			std::cout << "\t\t\t" << ((ser_it->second.m_Type == E_ERRORPAGE::REPLACE) ? ser_it->second.m_Replace : 0) << std::endl;
			}
			std::cout << "\n";

			for (auto loc_it = it->second.getLocationBlock().begin(); loc_it != it->second.getLocationBlock().end(); ++loc_it) {
				std::cout << "\t\t\t\t\tLocation Name: " << loc_it->first << std::endl;
				std::cout << "\t\t\t\t\t\tAccess log: " << loc_it->second.getAccess_log() << std::endl;
				std::cout << "\t\t\t\t\t\tRoot: " << loc_it->second.getRoot() << std::endl;
				std::cout << "\t\t\t\t\t\tAutoindex: " << (loc_it->second.getAutoindex() ? "on" : "off") << std::endl;
				std::cout << "\t\t\t\t\t\tIndex: " << loc_it->second.getIndex("domain1.com") << std::endl;
				for (auto err_it = loc_it->second.getError_page().begin(); err_it != loc_it->second.getError_page().end(); ++err_it) {
					std::cout << "\t\t\t\t\t" << err_it->first << ": " << (int)err_it->second.m_Type << " " << err_it->second.m_Path << std::endl;
					std::cout << "\t\t\t\t\t" << ((err_it->second.m_Type == E_ERRORPAGE::REPLACE) ? err_it->second.m_Replace : 0) << std::endl;
				}

			}
		}
	}

 }