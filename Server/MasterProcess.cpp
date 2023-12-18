#include "MasterProcess.hpp"

#include "../Parser/ConfParser/ConfData/ConfBlock.hpp"

// TODO: delete
#include <iostream>
#include <netinet/in.h>

typedef std::map<std::pair<std::string, unsigned short>, ft::shared_ptr<CONF::ServerBlock> > confServerBlockMaps;

MasterProcess::MasterProcess(const std::string& fileName, char** env) {
	try {
		CONF::ConfBlock::initInstance(fileName, env);
	} catch (ConfParserException& e) {
		std::cerr << e.getMessage() << std::endl;
	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}

MasterProcess::~MasterProcess() {
	std::cout << "MasterProcess destroyed\n";
	CONF::ConfBlock::getInstance()->destroy();
}

void	MasterProcess::start() {
	CONF::ConfBlock::getInstance()->print();

	const confServerBlockMaps&	mainServerBlocks = CONF::ConfBlock::getInstance()->getMainBlock().getHTTPBlock().getServerMap();
	unsigned short		curPort = 0;

	for (confServerBlockMaps::const_iterator it = mainServerBlocks.begin(); it != mainServerBlocks.end(); ++it) {
		// if (!curPort && curPort != it->second->getPort()) {
		// 	continue;
		// }
	}
}