#include "ConfMainBlock.hpp"
#include "../../ABNF_utils/ABNFFunctions.hpp"
#include "ConfBlock.hpp"

// TODO: delete
#include <iostream>

CONF::MainBlock::MainBlock()
: AConfParser(),
  m_BlockSwitch(false),
  m_Daemon(false),
  m_Status(0),
  m_Worker_process(4),
  m_Timer_resolution(0)
{
	// TODO: env parsing
	// CONF::ConfBlock::getInstance()->getShellEnv();
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

const bool	CONF::MainBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ENV: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Environment arguments!");
			} else {
				if (args.at(0).empty()) {
					throw ConfParserException(args.at(0), "invalid number of Environment arguments!");
				}
				std::pair<std::string, std::string>	env;
				this->m_Env.insert(env);
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::WORKER_PROCESS: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Worker Processes arguments!");
			} else {
				if (args.at(0).empty()) {
					throw ConfParserException(args.at(0), "invalid number of Worker Processes arguments!");
				}
				char*	endptr;
				const long	argumentNumber = std::strtol(args[0].c_str(), &endptr, 10);
				if (*endptr != '\0' || argumentNumber < 1) {
					throw ConfParserException(args[0], "invalid number of Worker Processes arguments!");
				}
				this->m_Worker_process = static_cast<unsigned int>(argumentNumber);
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::DAEMON: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Daemon arguments!");
			} else {
				if (args.at(0) == "on") {
					this->m_Daemon = true;
				} else if (args.at(0) == "off") {
					this->m_Daemon = false;
				} else {
					throw ConfParserException(args.at(0), "invalid number of Daemon arguments!");
				}
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Timer Resolution arguments!");
			} else {
				char*	endptr;
				const unsigned long	argumentNumber = static_cast<unsigned long>(std::strtol(args[0].c_str(), &endptr, 10));
				argumentNumber > 0 ? this->m_Timer_resolution = argumentNumber : throw ConfParserException(args.at(0), "invalid number of Timer Resolution arguments!");

				const std::string	time = args[0].substr(endptr - args[0].c_str(), args[0].length());
				// TODO: time check
				if (time == "ms") {
					;
				} else if (time == "s" || time.empty()) {
					this->m_Timer_resolution *= 1000;
				} else {
					throw ConfParserException(args.at(0), "invalid number of Timer Resolution arguments!");
				}
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG: {
			if (args.size() == 2 && args.at(1).empty()) {
				this->m_Error_log = args.at(0);
				return false;
			}
			throw ConfParserException(args.at(0), "invalid number of Error Log arguments!");
		}
		// case CONF::E_MAIN_BLOCK_STATUS::HTTP_BLOCK: {
		// 	if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
		// 		throw ConfParserException(args.at(0), "invalid number of HTTP arguments!");
		// 	}
		// 	m_BlockStack.push(CONF::E_BLOCK_STATUS::HTTP);
		// 	return true;
		// }
		case CONF::E_MAIN_BLOCK_STATUS::EVENT_BLOCK: {
			if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
				throw ConfParserException(args.at(0), "invalid number of Event arguments!");
			}
			this->m_BlockSwitch = true;
			return true;
		}
		default:
			throw ConfParserException("", "Invalid configure file!");
	}
}

const std::string	CONF::MainBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	std::string	argument;
	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}

	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG: {
			stringPathArgumentParser(argument);
			std::cout << "main arg: " << argument << std::endl;
			return (argument);
		}
	}
	while (Pos[E_INDEX::FILE] < fileSize && (std::isalnum(static_cast<int>(fileContent[Pos[E_INDEX::FILE]])) || fileContent[Pos[E_INDEX::FILE]] == '_' || fileContent[Pos[E_INDEX::FILE]] == '=')) {
		(std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) ? argument += std::tolower(fileContent[Pos[E_INDEX::FILE]]) : argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	return (argument);
}

const unsigned short	CONF::MainBlock::directiveNameChecker(const std::string& name) {
	const statusMap::iterator	it = m_MainStatusMap.find(name);

	if (it == m_MainStatusMap.end()) {
		throw ConfParserException(name, "main directive name is invalid!");
	} else {
		((m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), m_Status)) ? throw ConfParserException(name, "main directive is duplicated!") : m_Status |= it->second;
		return it->second;
	}
}

const bool	CONF::MainBlock::blockContent() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();


	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException("{", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	if (m_BlockSwitch) {
		std::cout << "Event Block" << std::endl;
		m_Event_block.initialize();
		std::cout << "Event Block End" << std::endl;
	} else {
		// m_HTTP_block.initialize();
	}
	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException("}", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}

const bool	CONF::MainBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| fileContent[Pos[E_INDEX::FILE]] == E_ABNF::LF) {
		return (false);
	}
	if (directives()) {
		return (blockContent());
	} else {
		return (ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]) ? true : false);
	}
}

void	CONF::MainBlock::initialize() {
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::MAIN);

	if (!contextLines()) {
		throw ConfParserException("", "is invalid Confgiure file!");
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

const std::string&	CONF::MainBlock::getErrorLog() {
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