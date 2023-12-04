#include "ConfParser.hpp"
#include "ConfParserUtils.hpp"
#include <cctype>
#include <string.h>
#include <string>
#include <vector>

// TODO: delete
#include <iostream>

CONF::ConfParser::ConfParser(const std::string& file)
: 
  m_FileName(file)
{
	const File	confFile(file);
	m_FileContent = confFile.getFileContent();
	m_FileSize = m_FileContent.length();
	m_Pos[E_INDEX::FILE] = 0;
	m_Pos[E_INDEX::LINE] = 1;
	m_Pos[E_INDEX::COLUMN] = 1;
	m_MainBlock = NULL;

	initStatusMap();
}

CONF::ConfParser::~ConfParser()
{
	m_MainBlock = NULL;
}


/**
 *				=========== Init Status Function ===========
*/
void	CONF::ConfParser::initMainStatusMap() {
	m_MainStatusMap["env"] = E_MAIN_BLOCK_STATUS::ENV;
	m_MainStatusMap["worker_processes"] = E_MAIN_BLOCK_STATUS::WORKER_PROCESS;
	m_MainStatusMap["daemon"] = E_MAIN_BLOCK_STATUS::DAEMON;
	m_MainStatusMap["timer_resolution"] = E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION;
	m_MainStatusMap["error_log"] = E_MAIN_BLOCK_STATUS::ERROR_LOG;
	m_MainStatusMap["worker_connections"] = E_MAIN_BLOCK_STATUS::WORKER_CONNECTIONS;
	m_MainStatusMap["http"] = E_MAIN_BLOCK_STATUS::HTTP_BLOCK;
	m_MainStatusMap["events"] = E_MAIN_BLOCK_STATUS::EVENT_BLOCK;
}

void	CONF::ConfParser::initEventStatusMap() {
	m_EventStatusMap["worker_connections"] = E_EVENT_BLOCK_STATUS::WORKER_CONNECTIONS;
}

void	CONF::ConfParser::initHTTPStatusMap() {
	m_HTTPstatusMap["root"] = E_HTTP_BLOCK_STATUS::ROOT;
	m_HTTPstatusMap["index"] = E_HTTP_BLOCK_STATUS::INDEX;
	m_HTTPstatusMap["autoindex"] = E_HTTP_BLOCK_STATUS::AUTOINDEX;
	m_HTTPstatusMap["error_page"] = E_HTTP_BLOCK_STATUS::ERROR_PAGE;
	m_HTTPstatusMap["access_log"] = E_HTTP_BLOCK_STATUS::ACCESS_LOG;
	m_HTTPstatusMap["keepalive_timeout"] = E_HTTP_BLOCK_STATUS::KEEPALIVE_TIMEOUT;
	m_HTTPstatusMap["include"] = E_HTTP_BLOCK_STATUS::INCLUDE;
	m_HTTPstatusMap["default_type"] = E_HTTP_BLOCK_STATUS::DEFAULT_TYPE;
	m_HTTPstatusMap["server"] = E_HTTP_BLOCK_STATUS::SERVER;
}

void	CONF::ConfParser::initServerStatusMap() {
	m_ServerStatusMap["root"] = E_SERVER_BLOCK_STATUS::ROOT;
	m_ServerStatusMap["index"] = E_SERVER_BLOCK_STATUS::INDEX;
	m_ServerStatusMap["autoindex"] = E_SERVER_BLOCK_STATUS::AUTOINDEX;
	m_ServerStatusMap["error_page"] = E_SERVER_BLOCK_STATUS::ERROR_PAGE;
	m_ServerStatusMap["access_log"] = E_SERVER_BLOCK_STATUS::ACCESS_LOG;
	m_ServerStatusMap["keepalive_timeout"] = E_SERVER_BLOCK_STATUS::KEEPALIVE_TIMEOUT;
	m_ServerStatusMap["listen"] = E_SERVER_BLOCK_STATUS::LISTEN;
	m_ServerStatusMap["server_name"] = E_SERVER_BLOCK_STATUS::SERVER_NAME;
	m_ServerStatusMap["location"] = E_SERVER_BLOCK_STATUS::LOCATION;
}

void	CONF::ConfParser::initLocationStatusMap() {
	m_LocationStatusMap["root"] = E_LOCATION_BLOCK_STATUS::ROOT;
	m_LocationStatusMap["index"] = E_LOCATION_BLOCK_STATUS::INDEX;
	m_LocationStatusMap["autoindex"] = E_LOCATION_BLOCK_STATUS::AUTOINDEX;
	m_LocationStatusMap["error_page"] = E_LOCATION_BLOCK_STATUS::ERROR_PAGE;
	m_LocationStatusMap["access_log"] = E_LOCATION_BLOCK_STATUS::ACCESS_PAGE;
	m_LocationStatusMap["internal"] = E_LOCATION_BLOCK_STATUS::INTERNAL;
	m_LocationStatusMap["cgi"] = E_LOCATION_BLOCK_STATUS::CGI;
	m_LocationStatusMap["location"] = E_LOCATION_BLOCK_STATUS::LOCATION;
}

void	CONF::ConfParser::initStatusMap() {
	initMainStatusMap();
	initEventStatusMap();
	initHTTPStatusMap();
	initServerStatusMap();
	initLocationStatusMap();
}

/**
 *				=========== Config Parsing Functions ===========
*/

/**
 *					[ File Name Parser ]
*/
const bool	CONF::ConfParser::fileName(std::string& argument) {
	while (m_Pos[E_INDEX::FILE] < m_FileSize && (std::isalnum(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) || m_FileContent[m_Pos[E_INDEX::FILE]] == '_' || m_FileContent[m_Pos[E_INDEX::FILE]] == '.')) {
		argument += m_FileContent[m_Pos[E_INDEX::FILE]];
		m_Pos[E_INDEX::FILE]++;
		m_Pos[E_INDEX::COLUMN]++;
	}
	if (argument.empty() || !std::isalpha(static_cast<int>((argument.rfind('.') + 1)))) {
		return false;
	}
	return true;
}


/**
 *					[ Argument Checker ]
 *		**** WARNING: if directive is block type, argumentChecker must return true. if not return false. ****
 *		server / location과 같은 directive는 stack에 status 넣은 후 구조체를 생성하여 push_back 해줘야 됨
 * 		location같은 경우 location에 중첩이 가능하기 때문에 stack에 status를 넣기 전, top을 확인하여 location에 넣을지, server에 넣을지 결정해야 됨
 *		server나 location이나 block에서 push_back한 것이기 때문에 directives나 argument에서 실제 값을 넣어줄 때는 vector의 맨 마지막만 확인하여 사용하면 된다.
 *		근데 location 중첩은 안 하는 것이 정신 건강에 좋을 법함. (실제 directives나 argument에서 값을 넣을 때 location의 얼마나 깊이에 있는 앤지 알 수가 있나 싶음)
*/
const bool	CONF::ConfParser::mainBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status) {
	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ENV: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Environment arguments!", m_Pos);
			} else {
				if (args.at(0).empty()) {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Environment arguments!", m_Pos);
				}
				std::pair<std::string, std::string>	env;
				m_MainBlock->m_Env.insert(env);
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::WORKER_PROCESS: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Worker Processes arguments!", m_Pos);
			} else {
				if (args.at(0).empty()) {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Worker Processes arguments!", m_Pos);
				}
				char*	endptr;
				const long	argumentNumber = std::strtol(args[0].c_str(), &endptr, 10);
				if (*endptr != '\0' || argumentNumber < 1) {
					throw ConfParserException(m_FileName, args[0], "invalid number of Worker Processes arguments!", m_Pos);
				}
				m_MainBlock->m_Worker_process = static_cast<unsigned int>(argumentNumber);
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::DAEMON: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Daemon arguments!", m_Pos);
			} else {
				if (args.at(0) == "on") {
					m_MainBlock->m_Daemon = true;
				} else if (args.at(0) == "off") {
					m_MainBlock->m_Daemon = false;
				} else {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Daemon arguments!", m_Pos);
				}
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::TIMER_RESOLUTION: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Timer Resolution arguments!", m_Pos);
			} else {
				char*	endptr;
				const unsigned long	argumentNumber = static_cast<unsigned long>(std::strtol(args[0].c_str(), &endptr, 10));
				argumentNumber > 0 ? m_MainBlock->m_Timer_resolution = argumentNumber : throw ConfParserException(m_FileName, args.at(0), "invalid number of Timer Resolution arguments!", m_Pos);

				const std::string	time = args[0].substr(endptr - args[0].c_str(), args[0].length());
				// TODO: time check
				if (time == "ms") {
					;
				} else if (time == "s" || time.empty()) {
					this->m_MainBlock->m_Timer_resolution *= 1000;
				} else {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Timer Resolution arguments!", m_Pos);
				}
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Error Log arguments!", m_Pos);
			}
			return false;
		}
		case CONF::E_MAIN_BLOCK_STATUS::HTTP_BLOCK: {
			if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of HTTP arguments!", m_Pos);
			}
			m_BlockStack.push(CONF::E_BLOCK_STATUS::HTTP);
			return true;
		}
		case CONF::E_MAIN_BLOCK_STATUS::EVENT_BLOCK: {
			if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Event arguments!", m_Pos);
			}
			m_BlockStack.push(CONF::E_BLOCK_STATUS::EVENT);
			return true;
		}
		default:
			throw ConfParserException(m_FileName, "", "Invalid configure file!", m_Pos);
	}
}

const bool	CONF::ConfParser::evnetsBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status) {
	switch (status) {
		case CONF::E_EVENT_BLOCK_STATUS::WORKER_CONNECTIONS: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Worker Connections arguments!", m_Pos);
			} else {
				if (args[0].empty()) {
					throw ConfParserException(m_FileName, args[0], "invalid number of Worker Connections arguments!", m_Pos);
				}
				char*	endptr;
				const long	argumentNumber = std::strtol(args[0].c_str(), &endptr, 10);
				if (*endptr != '\0' || argumentNumber < 1) {
					throw ConfParserException(m_FileName, args[0], "invalid number of Worker Connections arguments!", m_Pos);
				}
				m_MainBlock->m_Event_block.m_Worker_connections = static_cast<unsigned int>(argumentNumber);
			}
			return false;
		}
		default:
			throw ConfParserException(m_FileName, "", "Invalid event directive arguments!", m_Pos);
	}
}

const bool	CONF::ConfParser::httpBlockArgumentChecker(const std::vector<std::string>&args, const unsigned char& status) {
	switch (status) {
		case CONF::E_HTTP_BLOCK_STATUS::SERVER: {
			if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Server arguments!", m_Pos);
			}
			m_BlockStack.push(CONF::E_BLOCK_STATUS::SERVER);
			return true;
		}
		case CONF::E_HTTP_BLOCK_STATUS::INCLUDE: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Include arguments!", m_Pos);
			}
			// MIME type 파싱
			handleInclude(args[0]);
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::AUTOINDEX: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Autoindex arguments!", m_Pos);
			} else {
				if (args[0] == "on") {
					m_MainBlock->m_HTTP_block.m_Autoindex = true;
				} else if (args[0] == "off") {
					m_MainBlock->m_HTTP_block.m_Autoindex = false;
				} else {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Autoindex arguments!", m_Pos);
				}
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::ROOT: {
			if (args.size() != 1) {
				throw ConfParserException(m_FileName, args.at(0), "invalid number of Root arguments!", m_Pos);
			} else {
				if (args[0].empty()) {
					throw ConfParserException(m_FileName, args.at(0), "invalid number of Root arguments!", m_Pos);
				}
				size_t	uriPos = 0;
				URIParser::absPath(args[0], uriPos, m_MainBlock->m_HTTP_block.m_Root);
			}
			return false;
		}
		default:
			throw ConfParserException(m_FileName, "", "Invalid http directive arguments!", m_Pos);
	}
}

const bool	CONF::ConfParser::argumentChecker(const std::vector<std::string>& args, const unsigned char& status) {
	switch (m_BlockStack.top()) {
		case CONF::E_BLOCK_STATUS::MAIN: {
			return (mainBlockArgumentChecker(args, status));
		}
		case CONF::E_BLOCK_STATUS::EVENT: {
			return (evnetsBlockArgumentChecker(args, status));
		}
		case CONF::E_BLOCK_STATUS::HTTP: {
			return (httpBlockArgumentChecker(args, status));
		}
		case CONF::E_BLOCK_STATUS::SERVER: {
			return (serverBlockArgumentChecker(args, status));
		}
		case CONF::E_BLOCK_STATUS::LOCATION: {
			return (locationBlockArgumentChecker(args, status));
		}
		default:
			throw ConfParserException(m_FileName, "", "Invalid configure file!", m_Pos);
	}
}


// WARNING: 매개변수로 경로가 들어오면 무조건 앞에 '/'를 붙여준다.
// 따라서 나중에 경로를 탐색할 때 맨 앞의 인자가 empty면 절대경로이고, 아니면 상대경로이다.

/**
 *					[ Argument Utils ]
*/
void	CONF::ConfParser::absPathArgumentParser(strVec& argument) {
	const size_t		argumentLength = (std::strchr(&(m_FileContent[m_Pos[E_INDEX::FILE]]), E_ABNF::LF) - m_FileContent.c_str()) - m_Pos[E_INDEX::FILE] - 1;
	const std::string	uri = "/" + m_FileContent.substr(m_Pos[E_INDEX::FILE], argumentLength);
	size_t	uriPos = 0;
	URIParser::absPath(uri, uriPos, argument);
	m_Pos[E_INDEX::FILE] += argumentLength + 1;
	m_Pos[E_INDEX::COLUMN] += argumentLength + 1;
}

void	CONF::ConfParser::digitArgumentParser(std::string& argument) {
	while (m_Pos[E_INDEX::FILE] < m_FileSize && std::isdigit(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]]))) {
		argument += m_FileContent[m_Pos[E_INDEX::FILE]];
		m_Pos[E_INDEX::FILE]++;
		m_Pos[E_INDEX::COLUMN]++;
	}
	if (!ABNF::isWSP(m_FileContent, m_Pos[E_INDEX::FILE]) && m_FileContent[m_Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON && m_FileContent[m_Pos[E_INDEX::FILE]] != E_ABNF::LF) {
		throw ConfParserException(m_FileName, argument, "invalid type of Worker Connections arguments!", m_Pos);
	}
}

// DISCUSSION: index는 file이나 absPath가 올 수 있음.
// File || absPath 형식으로 해서 파싱을 호출할지
// 그냥 string으로 저장한 후 argumentChecker에서 처리할지
// 아래 error page와 같이 의논해야 함 (에러 페이지는 file, http://uri, absPath가 온다.)
void	CONF::ConfParser::indexArgumentParser(std::string& argument) {
}

// DISCUSSION: error_page는 http:// 스킴이 올 수 있음.. 
/*
	error_page 404 /not_found.html;
	error_page 500 501 502 503 504 /server_error.html;
	error_page 403 http://website.com/;
	error_page 404 @notfound; # 지정한 location 블록으로 이동
	error_page 404 =200 /index.html;

	=200이랑 @를 처리해야될까...?
	@는 location 블럭 탐색하여 처리하면 될 거 같은데 =200을 어떻게 처리할지 고민해봐야됨.
*/
void	CONF::ConfParser::errorPageArgumentParser(std::string& argument) {
	while (m_Pos[E_INDEX::FILE] < m_FileSize && (std::isalnum(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]]))
													|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_RESERVED::SLASH
													|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_RESERVED::COLON
													|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_MARK::UNDERSCORE
													|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_MARK::PERIOD))
	{
		argument += m_FileContent[m_Pos[E_INDEX::FILE]];
		m_Pos[E_INDEX::FILE]++;
		m_Pos[E_INDEX::COLUMN]++;
	}
}


/**
 *					[ Argument Parser ]
*/
const std::string	CONF::ConfParser::mainArgument(const unsigned short& status) {
	std::string	argument;

	switch (status) {
		case CONF::E_MAIN_BLOCK_STATUS::ERROR_LOG: {
			absPathArgumentParser(m_MainBlock->m_Error_log);
			break;
		}
		default:
			while (m_Pos[E_INDEX::FILE] < m_FileSize && (std::isalnum(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) || m_FileContent[m_Pos[E_INDEX::FILE]] == '_' || m_FileContent[m_Pos[E_INDEX::FILE]] == '=')) {
				(std::isalpha(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) ? argument += std::tolower(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) : argument += m_FileContent[m_Pos[E_INDEX::FILE]]);
				m_Pos[E_INDEX::FILE]++;
				m_Pos[E_INDEX::COLUMN]++;
			}
	}
	return argument;
}

const std::string	CONF::ConfParser::eventArgument(const unsigned short& status) {
	std:: string	argument;

	switch (status) {
		case CONF::E_EVENT_BLOCK_STATUS::WORKER_CONNECTIONS: {
			digitArgumentParser(argument);
			break;
		}
		default:
			throw ConfParserException(m_FileName, "", "Invalid event directive arguments!", m_Pos);
	}
	return argument;
}

const std::string	CONF::ConfParser::httpArgument(const unsigned short& status) {
	std::string	argument;

	switch (status) {
		case CONF::E_HTTP_BLOCK_STATUS::ROOT: {
			absPathArgumentParser(m_MainBlock->m_HTTP_block.m_Root);
			break;
		}
		case CONF::E_HTTP_BLOCK_STATUS::INDEX: {
			indexArgumentParser(argument);
			break;
		}
		case CONF::E_HTTP_BLOCK_STATUS::ERROR_PAGE: {
			errorPageArgumentParser(argument);
			break;
		}
		case CONF::E_HTTP_BLOCK_STATUS::ACCESS_LOG: {
			absPathArgumentParser(m_MainBlock->m_HTTP_block.m_Access_log);
			break;
		}
		// DISCUSSION: default와 합칠지 아니면 digit만 처리하는(event block) 형태로 둘지?
		case CONF::E_HTTP_BLOCK_STATUS::KEEPALIVE_TIMEOUT: {
			digitArgumentParser(argument);
			break;
		}
		case CONF::E_HTTP_BLOCK_STATUS::INCLUDE: {
			absPathArgumentParser(m_MainBlock->m_HTTP_block.m_Include);
			break;
		}
		// autoindex, server
		default:
			while (m_Pos[E_INDEX::FILE] < m_FileSize && (std::isalnum(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]]))
																		|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_RESERVED::SLASH
																		|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_RESERVED::COLON
																		|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_MARK::UNDERSCORE
																		|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_MARK::PERIOD
																		|| m_FileContent[m_Pos[E_INDEX::FILE]] == URI::E_MARK::HYPHEN))
			{
				(std::isalpha(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) ? argument += std::tolower(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) : argument += m_FileContent[m_Pos[E_INDEX::FILE]]);
				m_Pos[E_INDEX::FILE]++;
				m_Pos[E_INDEX::COLUMN]++;
			}
	}
	return argument;
}

const std::string	CONF::ConfParser::argument(const unsigned char& block_status, const unsigned short& status) {
	while (m_Pos[E_INDEX::FILE] < m_FileSize && ABNF::isWSP(m_FileContent, m_Pos[E_INDEX::FILE])) {
		m_Pos[E_INDEX::FILE]++;
		m_Pos[E_INDEX::COLUMN]++;
	}
	switch (block_status) {
		case CONF::E_BLOCK_STATUS::MAIN: {
			return mainArgument(status);
		}
		case CONF::E_BLOCK_STATUS::EVENT: {
			return eventArgument(status);
		}
		case CONF::E_BLOCK_STATUS::HTTP: {
			return httpArgument(status);
		}
		// case CONF::E_BLOCK_STATUS::SERVER: {
		// 	return serverArgument(status);
		// }
		// case CONF::E_BLOCK_STATUS::LOCATION: {
		// 	return locationArgument(status);
		// }
		default:
			throw ConfParserException(m_FileName, "", "Invalid configure file!", m_Pos);
	}
}

/**
 *					[ Directive Checker ]
 * 	@brief	Directive가 중복이 될 수 있는지 체크하는 함수
*/

const bool	CONF::ConfParser::isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status) {
	switch (block_status) {
		case CONF::E_BLOCK_STATUS::MAIN: {
			return (directive_status & CONF::E_MAIN_BLOCK_STATUS::ENV) ? true : false;
		}
		case CONF::E_BLOCK_STATUS::HTTP: {
			switch (directive_status) {
				case E_HTTP_BLOCK_STATUS::SERVER: {
					return true;
				}
				case E_HTTP_BLOCK_STATUS::INCLUDE: {
					return true;
				}
				default: {
					return false;
				}
			}
		}
		case CONF::E_BLOCK_STATUS::SERVER: {
			return (directive_status & CONF::E_SERVER_BLOCK_STATUS::LOCATION) ? true : false;
		}
		case CONF::E_BLOCK_STATUS::LOCATION: {
			return (directive_status & CONF::E_LOCATION_BLOCK_STATUS::LOCATION) ? true : false;
		}
		default:
			return false;
	}
}

/**
 *				Block마다 Directive가 올바른지 체크하는 함수들
 *		함수 생긴게 다 비슷한데 어떻게 한 번에 처리 못 할까?
*/
const unsigned short	CONF::ConfParser::mainDirectiveNameChecker(const std::string& name) {
	const statusShortMap::iterator&	it = m_MainStatusMap.find(name);
	if (it != m_MainStatusMap.end()) {
		((m_MainBlock->m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), it->second))
			? throw ConfParserException(m_FileName, name, "directive name is duplicated!", m_Pos) : m_MainBlock->m_Status |= it->second;
		return it->second;
	} else {
		throw ConfParserException(m_FileName, name, "main directive name is invalid!", m_Pos);
	}
}

const unsigned short	CONF::ConfParser::eventDirectiveNameChecker(const std::string& name) {
	const statusBoolMap::iterator&	it = m_EventStatusMap.find(name);
	if (it != m_EventStatusMap.end()) {
		((m_MainBlock->m_Event_block.m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), it->second))
			? throw ConfParserException(m_FileName, name, "directive name is duplicated!", m_Pos) : m_MainBlock->m_Event_block.m_Status |= it->second;
		return it->second;
	} else {
		throw ConfParserException(m_FileName, name, "event directive name is invalid!", m_Pos);
	}
}

const unsigned short	CONF::ConfParser::httpDirectiveNameChecker(const std::string& name) {
	const statusShortMap::iterator&	it = m_HTTPstatusMap.find(name);
	if (it != m_HTTPstatusMap.end()) {
		((m_MainBlock->m_HTTP_block.m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), it->second))
			? throw ConfParserException(m_FileName, name, "directive name is duplicated!", m_Pos) : m_MainBlock->m_HTTP_block.m_Status |= it->second;
		return it->second;
	} else {
		throw ConfParserException(m_FileName, name, "http directive name is invalid!", m_Pos);
	}
}

const unsigned short	CONF::ConfParser::serverDirectiveNameChecker(const std::string& name) {
	const statusShortMap::iterator&	it = m_ServerStatusMap.find(name);
	if (it != m_ServerStatusMap.end()) {
		((m_MainBlock->m_HTTP_block.m_Server_block.m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), it->second))
			? throw ConfParserException(m_FileName, name, "directive name is duplicated!", m_Pos) : m_MainBlock->m_HTTP_block.m_Server_block.m_Status |= it->second;
		return it->second;
	} else {
		throw ConfParserException(m_FileName, name, "server directive name is invalid!", m_Pos);
	}
}

const unsigned short	CONF::ConfParser::locationDirectiveNameChecker(const std::string& name) {

}

const unsigned short	CONF::ConfParser::directiveNameChecker(const std::string& name) {
	if (name.empty()) {
		throw ConfParserException(m_FileName, name, "directive name is empty!", m_Pos);
	}
	switch (m_BlockStack.top()) {
		case E_BLOCK_STATUS::MAIN:
			return mainDirectiveNameChecker(name);
		
		case E_BLOCK_STATUS::EVENT:
			return eventDirectiveNameChecker(name);

		case E_BLOCK_STATUS::HTTP:
			return httpDirectiveNameChecker(name);

		// case E_BLOCK_STATUS::SERVER:
		// 	return serverDirectiveNameChecker(name);

		// case E_BLOCK_STATUS::LOCATION:
		// 	return locationDirectiveNameChecker(name);

		default:
			throw ConfParserException(m_FileName, name, "directive name is invalid!", m_Pos);
	}
}

const unsigned short	CONF::ConfParser::directiveName() {
	std::string	name;

	while (m_Pos[E_INDEX::FILE] < m_FileSize && (std::isalpha(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) || m_FileContent[m_Pos[E_INDEX::FILE]] == '_')) {
		(std::isalpha(static_cast<int>(m_FileContent[m_Pos[E_INDEX::FILE]])) ? name += std::tolower(m_FileContent[m_Pos[E_INDEX::FILE]]) : name += m_FileContent[m_Pos[E_INDEX::FILE]]);
		m_Pos[E_INDEX::FILE]++;
		m_Pos[E_INDEX::COLUMN]++;
	}
	return (directiveNameChecker(name));
}

const bool	CONF::ConfParser::directives() {
	if (m_FileContent[m_Pos[E_INDEX::FILE]] == E_CONF::RBRACE) {
		return (false);
	}
	const unsigned short&		status = directiveName();

	std::vector<std::string>	args;
	while (m_Pos[E_INDEX::FILE] < m_FileSize && m_FileContent[m_Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON && m_FileContent[m_Pos[E_INDEX::FILE]] != E_ABNF::LF && m_FileContent[m_Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		args.push_back(argument(m_BlockStack.top(), status));
	}
	return (argumentChecker(args, status));
}


/**
 *					[ Context Checker ]
*/
const bool	CONF::ConfParser::blockContent() {
	if (m_FileContent[m_Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException(m_FileName, "{", "Direct block has no brace!", m_Pos);
	}
	m_Pos[E_INDEX::FILE]++;
	m_Pos[E_INDEX::COLUMN]++;
	while (m_Pos[E_INDEX::FILE] < m_FileSize && m_FileContent[m_Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		contextLines();
		continue ;
	}
	if (m_FileContent[m_Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException(m_FileName, "}", "Direct block has no brace!", m_Pos);
	}
	m_Pos[E_INDEX::FILE]++;
	m_Pos[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}

const bool	CONF::ConfParser::context() {
	if (m_FileContent[m_Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON || m_FileContent[m_Pos[E_INDEX::FILE]] == E_ABNF::LF) {
		return (false);
	}
	if (directives()) {
		return (blockContent());
	} else {
		return (ABNF::isC_nl(m_FileContent, m_Pos[E_INDEX::FILE]) ? true : false);
	}
}

const bool	CONF::ConfParser::contextLines() {
	while (m_Pos[E_INDEX::FILE] < m_FileSize) {
		while (m_Pos[E_INDEX::FILE] < m_FileSize && ABNF::isWSP(m_FileContent, m_Pos[E_INDEX::FILE])) {
			m_Pos[E_INDEX::FILE]++;
			m_Pos[E_INDEX::COLUMN]++;
		}
		if (context() || ABNF::isC_nl(m_FileContent, m_Pos[E_INDEX::FILE])) {
			m_Pos[E_INDEX::LINE]++;
			m_Pos[E_INDEX::COLUMN] = 1;
		} else {
			return (false);
		}
	}
	return true;
}

/**
 *				=========== Beginning of Parsing ===========
*/
void	CONF::ConfParser::Parsing(CONF::MainBlock& mainBlock) {
	m_BlockStack.push(CONF::E_BLOCK_STATUS::MAIN);
	m_MainBlock = &mainBlock;
	bzero(&m_MainBlock->m_Status, sizeof(m_MainBlock->m_Status));
	if (!contextLines()) {
		throw ConfParserException(m_FileName, "", "Configure file!", m_Pos);
	}
}