#include "../ConfParserHeaders/ConfMainBlock.hpp"
#include "../../../File/File.hpp"
#include <cstddef>
#include <cstdlib>

#include <iostream>

// CONF::httpBlock		CONF::mainBlock::m_Http_block;		     
CONF::mainBlock*	CONF::mainBlock::instance = NULL;

/**
 * public function
 * @brief	Singleton Instance
 * @details	싱글톤 패턴을 사용하여 하나의 인스턴스만 생성
 * initInstance()를 통해 인스턴스를 생성
 * getInstance()를 통해 인스턴스를 반환
 */
CONF::mainBlock*	CONF::mainBlock::initInstance(const std::string& file) {
	if (!CONF::mainBlock::instance) {
		CONF::mainBlock::instance = new CONF::mainBlock(file);
	}
	return CONF::mainBlock::instance;
}

CONF::mainBlock*	CONF::mainBlock::getInstance() {
	return CONF::mainBlock::instance;
}

/**
 * private function
 * @brief	Constructor / Destructor
 * @details	생성자에서는 파일을 읽어들여 파싱을 진행
 * 파싱을 진행하면서 각각의 디렉티브에 맞는 함수를 호출
 */
CONF::mainBlock::mainBlock(const std::string& file)
 : m_Daemon(false),
	m_Worker_process(4),
	m_Time_resolution(100),
	m_FileName(file)
{
	const File	confFile(file);
	size_t		index[3];
	index[CONF::POS] = 0;
	index[CONF::LINE] = 1;
	index[CONF::COLUMN] = 1;
	
	initStatusMap();
	contextLines(confFile.getFileContent(), index);
}

CONF::mainBlock::~mainBlock(void) {}


/**
 * @brief	public function
 * @details	맴버 변수들을 반환하는 getter 함수
 */
// const CONF::httpBlock&	CONF::mainBlock::getHttpBlock() {
// 	return ;
// }

const bool	CONF::mainBlock::isDaemonOn() {
	return this->m_Daemon;
}

const unsigned int&	CONF::mainBlock::getWorkerProcess() {
	return this->m_Worker_process;
}

const unsigned long&	CONF::mainBlock::getTimeResolution() {
	return this->m_Time_resolution;
}

const std::vector<std::string>&	CONF::mainBlock::getErrorLog() {
	return this->m_Error_log;
}

const std::string&	CONF::mainBlock::getEnv(const std::string& key) {
	envMap::iterator	it = this->m_Env.find(key);
	if (it != this->m_Env.end()) {
		return it->second;
	} else {
		throw ConfParserException(m_FileName, key, "cannot find Environment.", NULL);
	}
}

const CONF::mainBlock::envMap&	CONF::mainBlock::getEnvMap() {
	return this->m_Env;
}

const unsigned int&	CONF::mainBlock::getWorkerConnections() {
	return this->m_Event_block.worker_connections;
}


/**
 * @brief	Configure Parser functions
 */

void	CONF::mainBlock::initStatusMap(void) {
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("env", CONF::E_MAIN_BLOCK_STATUS::ENV));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("worker_processes", CONF::E_MAIN_BLOCK_STATUS::WORKER_PROCESS));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("daemon", CONF::E_MAIN_BLOCK_STATUS::DAEMON));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("timer_resolution", CONF::E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("error_log", CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("http", CONF::E_MAIN_BLOCK_STATUS::HTTP_BLOCK));
	this->m_StatusMap.insert(std::pair<std::string, unsigned char>("events", CONF::E_MAIN_BLOCK_STATUS::EVENT_BLOCK));
}

unsigned char	CONF::mainBlock::directiveNameChecker(const std::string& name, size_t* index) {
	if (name.empty()) {
		throw ConfParserException(m_FileName, name, "directive name is empty.", index);
	}
	const statusMap::iterator it = m_StatusMap.find(name);
	if (it != m_StatusMap.end()) {
		// TODO : 중복 directive일 수도 있어야 함
		(m_Status & it->second) ? throw (ConfParserException(m_FileName, name, "directive is duplicated.", index)) : m_Status |= it->second;
		return it->second;
	} else {
		throw ConfParserException(m_FileName, name, "invalid directive name.", index);
	}
}

void	CONF::mainBlock::argumentChecker(const std::vector<std::string>& args, const unsigned char& status, const size_t* index) {
	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ENV: 
			if (args.size()!= 1) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
			} else {
				std::pair<std::string, std::string>	env; // = envParser();
				this->m_Env.insert(std::pair<std::string, std::string>(env));
			}
			break;

		case CONF::E_MAIN_BLOCK_STATUS::WORKER_PROCESS: {
			char*	end;
			if (args.size()!= 1) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
		    } else {
				const long argumentNumber = static_cast<unsigned int>(std::strtol(args[0].c_str(), &end, 10));
				if (*end != '\0' || argumentNumber < 1) {
					throw ConfParserException(m_FileName, args[0], "worker_processes argument should not include string or not be negative.", index);
				}
				(argumentNumber > 0) ? this->m_Worker_process = argumentNumber : throw (ConfParserException(m_FileName, args[0], "invalid argument.", index));
			}
			break;

		}
		case CONF::E_MAIN_BLOCK_STATUS::DAEMON:
			if (args.size()!= 1) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
		    } else {
				if (args[0] == "on") {
					m_Daemon = true;
				} else if (args[0] == "off") {
					;
				} else {
					throw ConfParserException(m_FileName, args[0], "invalid argument.", index);
				}
			}
			break;

		case CONF::E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION: {
			char*	end;
			if (args.size()!= 1) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
			} else {
                const long argumentNumber = static_cast<unsigned int>(std::strtol(args[0].c_str(), &end, 10));
                argumentNumber > 0? this->m_Time_resolution = argumentNumber : throw (ConfParserException(m_FileName, args[0], "invalid argument.", index));
				
				const std::string	time = args[0].substr((end - args[0].c_str()), args[0].length());
				// TODO: time check			
				if (time == "ms") {
					this->m_Time_resolution *= 1000;
				} else if (time == "s" || time.empty()) {
					;
				} else {
					throw ConfParserException(m_FileName, args[0], "invalid argument.", index);
				}
			}
			break;
		}

		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG:
			if (args.size()!= 1) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
			}
			break;

		case CONF::E_MAIN_BLOCK_STATUS::HTTP_BLOCK:
			if (!args.empty()) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
			}
			break;
			// this->m_Http_block = httpParser();

		case CONF::E_MAIN_BLOCK_STATUS::EVENT_BLOCK:
			if (!args.empty()) {
				throw ConfParserException(m_FileName, args[0], "invalid number of arguments.", index);
			}
			break;
			// this->m_Event_block = eventParser();

		default:
			throw ConfParserException(m_FileName, args[0], "invalid directive name.", index);
	}
}

std::string	CONF::mainBlock::argument(const std::string& file, size_t* index, const unsigned char& status) {
	std::string	argument;
	
	while (index[CONF::POS] < file.length() && ABNF::isWSP(file, index[CONF::POS])) {
		index[CONF::POS]++;
		index[CONF::COLUMN]++;
	}
	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG: {
			const size_t	argumentLength = (std::strchr(&(file[index[CONF::POS]]), E_ABNF::LF) - file.c_str()) - index[CONF::POS] - 1;
			const std::string uri = "/" + file.substr(index[CONF::POS], argumentLength);
			size_t	uriPos = 0;
			URIParser::absPath(uri, uriPos, m_Error_log);
			index[CONF::POS] += argumentLength + 1;
			index[CONF::COLUMN] += argumentLength + 1;
			break;
		}
		default:
			while (index[CONF::POS] < file.size() && (std::isalnum(static_cast<unsigned char>(file[index[CONF::POS]])) || file[index[CONF::POS]] == '_' || file[index[CONF::POS]] == '=')) {
				(std::isalpha(static_cast<int>(file[index[CONF::POS]])) ? argument += std::tolower(static_cast<int>(file[index[CONF::POS]])) : argument += file[index[CONF::POS]]);
				index[CONF::POS]++;
				index[CONF::COLUMN]++;
			}
	}
	return argument;
}

unsigned char	CONF::mainBlock::directiveName(const std::string& file, size_t* index) {
	std::string	name;

	while (index[CONF::POS] < file.size() && (std::isalpha(static_cast<unsigned char>(file[index[CONF::POS]])) || file[index[CONF::POS]] == '_')) {
		(std::isalpha(static_cast<unsigned char>(file[index[CONF::POS]])) ? name += std::tolower(file[index[CONF::POS]]) : name += file[index[CONF::POS]]);
		index[CONF::POS]++;
		index[CONF::COLUMN]++;
	}
	return (directiveNameChecker(name, index));
}

void	CONF::mainBlock::directives(const std::string& file, size_t* index) {
	const unsigned char		directiveStatus = directiveName(file, index);
	
	std::vector<std::string>	args;
	while (index[CONF::POS] < file.size() && file[index[CONF::POS]] != E_ABNF::SEMICOLON && file[index[CONF::POS]] != E_ABNF::LF) {
		args.push_back(argument(file, index, directiveStatus));
	}
	argumentChecker(args, directiveStatus, index);
}

bool	CONF::mainBlock::blockContent(const std::string& file, size_t* index) {
	return true;
}

bool	CONF::mainBlock::context(const std::string& file, size_t* index) {
	if (file[index[CONF::POS]] == E_ABNF::SEMICOLON || file[index[CONF::POS]] == E_ABNF::LF) {
		return false;
	}
	directives(file, index);
	ABNF::isC_nl(file, index[CONF::POS]);
	// if (blockContent(file, index) || ABNF::isC_nl(file, index)) {
	// 	return true;
	// } else {
	// 	return false;
	// }
	return true;
}

void	CONF::mainBlock::contextLines(const std::string& file, size_t* index) {
	while (index[CONF::POS] < file.size()) {
		while (index[CONF::POS] < file.size() && ABNF::isWSP(file, index[CONF::POS])) {
			index[CONF::POS]++;
			index[CONF::COLUMN]++;
		}
		if (context(file, index) || ABNF::isC_nl(file, index[CONF::POS])) {
			index[CONF::LINE]++;
			index[CONF::COLUMN] = 1;
		} else {
			throw ConfParserException(file, "conf main block", "is invalid context.", index);
		}
	}
}


/**
 * @brief	Debug
 * 
 */
 void	CONF::mainBlock::print() {
	std::cout << "Main Block" << std::endl;
	std::cout << "\tEnv: " << std::endl;
    for (const auto& env : m_Env) {
		std::cout << "\t\t" << env.first << "=" << env.second << std::endl;
	}
	std::cout << "\tWorker_process: " << m_Worker_process << std::endl;
    std::cout << "\tDaemon: " << (m_Daemon? "on" : "off") << std::endl;
    std::cout << "\tTime_resolution: " << m_Time_resolution << std::endl;
	for (const auto& log : m_Error_log) {
		std::cout << "\tError_log: " << log << std::endl;
	}
 }