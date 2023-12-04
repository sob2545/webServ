#include "ConfMainBlock.hpp"
#include <atomic>

CONF::MainBlock::MainBlock()
: ConfParser(),
  m_EventOrHttp(false),
  m_Daemon(false),
  m_Status(0),
  m_Worker_process(4),
  m_Timer_resolution(0)
{
	 initMainStatusMap();
}

CONF::MainBlock::~MainBlock() {}

void	CONF::MainBlock::initMainStatusMap() {
	m_MainStatusMap["env"] = E_MAIN_BLOCK_STATUS::ENV;
	m_MainStatusMap["worker_processes"] = E_MAIN_BLOCK_STATUS::WORKER_PROCESS;
	m_MainStatusMap["daemon"] = E_MAIN_BLOCK_STATUS::DAEMON;
	m_MainStatusMap["timer_resolution"] = E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION;
	m_MainStatusMap["error_log"] = E_MAIN_BLOCK_STATUS::ERROR_LOG;
	m_MainStatusMap["worker_connections"] = E_MAIN_BLOCK_STATUS::WORKER_CONNECTIONS;
	m_MainStatusMap["http"] = E_MAIN_BLOCK_STATUS::HTTP_BLOCK;
	m_MainStatusMap["events"] = E_MAIN_BLOCK_STATUS::EVENT_BLOCK;
}



const unsigned short	CONF::MainBlock::directiveNameChecker() {
	
}

const bool	CONF::MainBlock::blockContent() {
	if (CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException(CONF::ConfParser::fileName(), "{", "Direct block has no brace!", CONF::ConfParser::Pos());
	}
	CONF::ConfParser::Pos()[E_INDEX::FILE]++;
	CONF::ConfParser::Pos()[E_INDEX::COLUMN]++;
	if (m_EventOrHttp) {
		m_Event_block.initialize();
	} else {
		m_HTTP_block.initialize();
	}
	if (CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException(CONF::ConfParser::fileName(), "}", "Direct block has no brace!", CONF::ConfParser::Pos());
	}
	CONF::ConfParser::Pos()[E_INDEX::FILE]++;
	CONF::ConfParser::Pos()[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}

const bool	CONF::MainBlock::context() {
	if (CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]] == E_ABNF::LF) {
		return (false);
	}
	if (directives()) {
		return (blockContent());
	} else {
		return (ABNF::isC_nl(CONF::ConfParser::fileContent(), CONF::ConfParser::Pos()[E_INDEX::FILE]) ? true : false);
	}
}

void	CONF::MainBlock::initialize() {
	CONF::ConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::MAIN);

	if (!contextLines()) {
		throw ConfParserException(CONF::ConfParser::fileName(), "", "is invalid Confgiure file!", CONF::ConfParser::Pos());
	}
}

//			Getter Functions

const bool	CONF::MainBlock::isDaemonOn() {
	return this->m_Daemon;
}

const unsigned int&	CONF::MainBlock::getWorkerProcess() {
	return this->m_Worker_process;
}

const unsigned long&	CONF::MainBlock::getTimeResolution() {
	return this->m_Timer_resolution;
}

const CONF::MainBlock::strVec&	CONF::MainBlock::getErrorLog() {
	return this->m_Error_log;
}

const CONF::MainBlock::envMap&	CONF::MainBlock::getEnvMap() {
	return this->m_Env;
}

const std::string&	CONF::MainBlock::getEnv(const std::string& key) {
	return this->m_Env[key];
}

const unsigned int&	CONF::MainBlock::getWorkerConnections() {
	return this->m_Event_block.m_Worker_connections;
}