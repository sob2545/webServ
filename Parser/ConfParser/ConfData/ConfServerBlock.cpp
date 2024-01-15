#include "ConfServerBlock.hpp"

// TODO: delete
#include <iostream>
#include <sys/_types/_size_t.h>

std::map<std::string, unsigned short>	CONF::ServerBlock::m_ServerStatusMap;

CONF::ServerBlock::ServerBlock(
	const bool&				autoIndex,
	const unsigned int&		keepAliveTime,
	const std::string&		root,
	const std::string&		accessLog,
	const ErrorPageMap_t&	errorPage,
	const Trie&				index
)
: AConfParser(),
  m_Autoindex(autoIndex),
  m_Port(80),
  m_Status(0),
  m_KeepAliveTime(keepAliveTime),
  m_Root(root),
  m_Error_page(errorPage),
  m_Access_log(accessLog),
  m_IP(""),
  m_Index(index)
{}

CONF::ServerBlock::ServerBlock(const ServerBlock& other)
 : AConfParser(),
   m_Autoindex(other.m_Autoindex),
   m_Port(other.m_Port),
   m_Status(other.m_Status),
   m_KeepAliveTime(other.m_KeepAliveTime),
   m_Root(other.m_Root),
   m_Error_page(other.m_Error_page),
   m_Access_log(other.m_Access_log),
   m_IP(other.m_IP),
   m_Index(other.m_Index),
   m_LocationName(other.m_LocationName),
   m_Server_name(other.m_Server_name),
   m_LocationBlock(other.m_LocationBlock)
{}

CONF::ServerBlock::~ServerBlock() {}

void	CONF::ServerBlock::initServerStatusMap() {
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


bool	CONF::ServerBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	switch (status) {
		case CONF::E_SERVER_BLOCK_STATUS::ROOT: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Root arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args.at(0), "invalid number of Root arguments!") : this->m_Root = args[0];
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::INDEX: {
			args.empty() ? throw ConfParserException("", "index argument is empty!") : 0;
			for (std::size_t i = 0; i < args.size(); i++) {
				(args[i].empty()) ? throw ConfParserException(args.at(0), "invalid number of Index arguments!") : this->m_Index.insert(args[i]);
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::AUTOINDEX: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Autoindex arguments!");
			} else {
				if (args.at(0).empty()) {
					throw ConfParserException(args.at(0), "invalid number of Autoindex arguments!");
				}
				if (args.at(0) == "on") {
					this->m_Autoindex = true;
				} else if (args.at(0) == "off") {
					this->m_Autoindex = false;
				} else {
					throw ConfParserException(args.at(0), "invalid number of Autoindex arguments!");
				}
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::ERROR_PAGE: {
			errorPageChecker(args, this->m_Error_page);
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::ACCESS_LOG: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Access Log arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args[0], "invalid number of Access Log arguments!") : this->m_Access_log = args[0];
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::KEEPALIVE_TIMEOUT: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Keepalive Timeout arguments!");
			} else {
				(args[0].size() > 9) ? throw ConfParserException(args[0], "is invalid Keepalive Timeout argument!") : 0;
				const int sec = std::atoi(args[0].c_str());
				(sec < 0) ? throw ConfParserException(args.at(0), "invalid number of Keepalive Timeout arguments!") : this->m_KeepAliveTime = sec;
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::LISTEN: {
			if (args.size() > 1) {
				throw ConfParserException(args.at(0), "invalid number of Listen arguments!");
			} else if (args.empty()) {
				const unsigned short 	listenCheckBit = 1 << 15;
				m_Status & listenCheckBit ? m_IP.clear() : throw ConfParserException("", "listen argument is empty!");
			} else {
				this->m_IP = args[0];
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::SERVER_NAME: {
			args.empty() ? throw ConfParserException("", "server_name argument is empty!") : 0;
			for (std::size_t i = 0; i < args.size(); i++) {
				(args[i].empty()) ? throw ConfParserException(args.at(0), "invalid number of Index arguments!") : this->m_Server_name.insert(args[i]);
			}
			return false;
		}
		case CONF::E_SERVER_BLOCK_STATUS::LOCATION: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Location arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args[0], "invalid number of Access Log arguments!") : this->m_LocationName = args[0];
			}
			return true;
		}
	}
	throw ConfParserException("", "Invalid configure file!");
}

const std::string	CONF::ServerBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	std::string	argument;
	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		handleHtabSpace(fileContent.at(Pos[E_INDEX::FILE]));
		Pos[E_INDEX::FILE]++;
	}

	switch (status) {
		case CONF::E_SERVER_BLOCK_STATUS::ROOT:
		case CONF::E_SERVER_BLOCK_STATUS::INDEX:
		case CONF::E_SERVER_BLOCK_STATUS::ACCESS_LOG:
			return (stringPathArgumentParser(argument) ? argument : throw ConfParserException(argument, "invalid root argument format!"));
		case CONF::E_SERVER_BLOCK_STATUS::LOCATION: {
			if (fileContent[Pos[E_INDEX::FILE]] == E_CONF::LBRACE) {
				return (argument);
			}
			return (stringPathArgumentParser(argument) ? argument : throw ConfParserException(argument, "invalid root argument format!"));
		}
		case CONF::E_SERVER_BLOCK_STATUS::ERROR_PAGE: {
			errorPageArgumentParser(argument);
			return (argument);
		}
		case CONF::E_SERVER_BLOCK_STATUS::KEEPALIVE_TIMEOUT: {
			if (!digitArgumentParser(argument)) {
				throw ConfParserException(argument, "invalid number of Keepalive Timeout arguments!");
			}
			return (argument);
		}
		case CONF::E_SERVER_BLOCK_STATUS::SERVER_NAME: {
			const std::size_t	startPos = Pos[E_INDEX::FILE];
			URIParser::hostnameParser<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument, this->m_Port);
			Pos[E_INDEX::COLUMN] += (Pos[E_INDEX::FILE] - startPos);
			return (argument);
		}
		case CONF::E_SERVER_BLOCK_STATUS::LISTEN: {
			const std::size_t		startPos = Pos[E_INDEX::FILE];
			const unsigned short 	listenCheckBit = 1 << 15;

			URIParser::IPv4Parser<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument, this->m_Port) ? this->m_Status |= listenCheckBit : 0;
			Pos[E_INDEX::COLUMN] += (Pos[E_INDEX::FILE] - startPos);
			return (argument);
		}
	}
	argumentParser(argument);
	return (argument);
}


unsigned short	CONF::ServerBlock::directiveNameChecker(const std::string& name) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const StatusMap_t::iterator	it = m_ServerStatusMap.find(name);

	if (it == m_ServerStatusMap.end()) {
		throw ConfParserException(name, "Server directive name is invalid!");
	} else {
		((m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), m_Status)) ? throw ConfParserException(name, "Server directive is duplicated!") : m_Status |= it->second;
		return it->second;
	}
}

bool	CONF::ServerBlock::blockContent() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();


	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException("{", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;

	LocationBlock	locationBlock(this->m_Autoindex,
									this->m_Root,
									this->m_Access_log,
									this->m_Error_page,
									this->m_Index);
	locationBlock.initialize();

	m_LocationBlock.insert(std::make_pair(m_LocationName, locationBlock));

	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException("}", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}


bool	CONF::ServerBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*		Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| ABNF::isLF(fileContent, Pos[E_INDEX::FILE])) {
		return (false);
	}
	if (directives()) {
		return (blockContent());
	} else {
		return (ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]) ? true : false);
	}
}

void	CONF::ServerBlock::initialize() {
	this->m_ServerStatusMap.empty() ? initServerStatusMap() : static_cast<void>(0);
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::SERVER);

	contextLines();
}


const bool&	CONF::ServerBlock::getAutoindex() const {
	return (this->m_Autoindex);
}

const std::string&	CONF::ServerBlock::getRoot() const {
	return (this->m_Root);
}

const unsigned short&	CONF::ServerBlock::getPort() const {
	return (this->m_Port);
}

const std::string&	CONF::ServerBlock::getIP() const {
	return (this->m_IP);
}

const std::string	CONF::ServerBlock::getIndex(const std::string& uri) const {
	return (this->m_Index.find(uri));
}

const std::string&	CONF::ServerBlock::getAccess_log() const {
	return (this->m_Access_log);
}

const unsigned int&	CONF::ServerBlock::getKeepAliveTime() const {
	return (this->m_KeepAliveTime);
}

const std::map<unsigned short, CONF::errorPageData>&	CONF::ServerBlock::getError_page() const {
	return (this->m_Error_page);
}

const std::set<std::string>&	CONF::ServerBlock::getServerNames() const {
	return (this->m_Server_name);
}

const CONF::ServerBlock::LocationBlockMap_t&	CONF::ServerBlock::getLocationMap() const {
	return (this->m_LocationBlock);
}
