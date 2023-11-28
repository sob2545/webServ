#include "ConfMainBlock.hpp"

CONF::httpBlock		CONF::mainBlock::m_Http_block;             
CONF::mainBlock*	CONF::mainBlock::instance = NULL;

// public
CONF::mainBlock*	CONF::mainBlock::initInstance(const std::string& file) {
	if (!CONF::mainBlock::instance) {
		CONF::mainBlock::instance = new CONF::mainBlock(file);
	}
	return CONF::mainBlock::instance;
}

CONF::mainBlock*	CONF::mainBlock::getInstance() {
	return CONF::mainBlock::instance;
}

// private
CONF::mainBlock::mainBlock(const std::string& file)
 : m_Daemon(false),
	m_Worker_process(4),
	m_Time_resolution(100)
{
	size_t	pos(0);

	contextLines(file, pos);
}

CONF::mainBlock::~mainBlock(void) {}

// public
const CONF::httpBlock&	CONF::mainBlock::getHttpBlock() {
	return ;
}

const bool	CONF::mainBlock::isDaemonOn() {
	return this->m_Daemon;
}

const unsigned int&	CONF::mainBlock::getWorkerProcess() {
	return this->m_Worker_process;
}

const unsigned long&	CONF::mainBlock::getTimeResolution() {
	return this->m_Time_resolution;
}

const std::string&	CONF::mainBlock::getErrorLog() {
	return this->m_Error_log;
}

const std::string&	CONF::mainBlock::getEnv(const std::string& key) {
	envMap::iterator	it = this->m_Env.find(key);
	if (it != this->m_Env.end()) {
		return it->second;
	} else {
		throw (Utils::errorMessageGenerator(key, 0, "cannot find Environment."));
	}
}

const CONF::mainBlock::envMap&	CONF::mainBlock::getEnvMap() {
	return this->m_Env;
}

const unsigned int&	CONF::mainBlock::getWorkerConnections() {
	return this->m_Event_block.worker_connections;
}

// TODO : directives
bool	CONF::mainBlock::blockContent(const std::string& file, size_t& pos) {
	directives(file, pos);
}

bool	CONF::mainBlock::simpleContent(const std::string& file, size_t& pos) {

}

bool	CONF::mainBlock::context(const std::string& file, size_t& pos) {
	if (blockContent(file, pos) || simpleContent(file, pos)) {
		return true;
	} else {
		return false;
	}
}

void	CONF::mainBlock::contextLines(const std::string& file, size_t& pos) {
	while (pos < file.size() && ABNF::isWSP(file, pos)) {
		if (!context(file, pos) && !ABNF::isC_nl(file, pos)) {
			throw (Utils::errorMessageGenerator(file, pos, "is invalid context line."));
		}
	}
}
