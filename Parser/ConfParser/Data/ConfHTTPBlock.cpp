#include "ConfHTTPBlock.hpp"
#include <string>
#include "../../MIMEParser/MIMEParser.hpp"
#include "ConfServerBlock.hpp"
#include "errorPageData/errorPageData.hpp"

// TODO: delete
#include <iostream>
#include <sys/_types/_size_t.h>
#include <utility>

std::map<std::string, unsigned short>	CONF::HTTPBlock::m_HTTPStatusMap;

CONF::HTTPBlock::HTTPBlock()
: AConfParser(),
  m_Autoindex(false),
  m_Status(0),
  m_KeepAliveTime(75),
  m_Default_type("text/plain")
{}

CONF::HTTPBlock::~HTTPBlock() {
}

void	CONF::HTTPBlock::initHTTPStatusMap() {
	m_HTTPStatusMap["root"] = E_HTTP_BLOCK_STATUS::ROOT;
	m_HTTPStatusMap["index"] = E_HTTP_BLOCK_STATUS::INDEX;
	m_HTTPStatusMap["autoindex"] = E_HTTP_BLOCK_STATUS::AUTOINDEX;
	m_HTTPStatusMap["error_page"] = E_HTTP_BLOCK_STATUS::ERROR_PAGE;
	m_HTTPStatusMap["access_log"] = E_HTTP_BLOCK_STATUS::ACCESS_LOG;
	m_HTTPStatusMap["keepalive_timeout"] = E_HTTP_BLOCK_STATUS::KEEPALIVE_TIMEOUT;
	m_HTTPStatusMap["include"] = E_HTTP_BLOCK_STATUS::INCLUDE;
	m_HTTPStatusMap["default_type"] = E_HTTP_BLOCK_STATUS::DEFAULT_TYPE;
	m_HTTPStatusMap["server"] = E_HTTP_BLOCK_STATUS::SERVER;
}


bool	CONF::HTTPBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	switch (status) {
		case CONF::E_HTTP_BLOCK_STATUS::ROOT: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Root arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args.at(0), "invalid number of Root arguments!") : this->m_Root = args[0];
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::INDEX: {
			args.empty() ? throw ConfParserException("", "index argument is empty!") : 0;
			for (std::size_t i = 0; i < args.size(); i++) {
				(args[i].empty()) ? throw ConfParserException(args.at(0), "invalid number of Index arguments!") : this->m_Index.insert(args[i]);
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::AUTOINDEX: {
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
		case CONF::E_HTTP_BLOCK_STATUS::ERROR_PAGE: {
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
				const int sec = std::atoi(args[0].c_str());
				(sec < 0) ? throw ConfParserException(args.at(0), "invalid number of Keepalive Timeout arguments!") : this->m_KeepAliveTime = sec;
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::INCLUDE: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Include arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args[0], "invalid number of Include arguments!") : 0;
				const std::size_t		dotPos = args[0].rfind('.');
				const std::string&	extension = args[0].substr(dotPos + 1, args[0].size() - dotPos);
				if (extension == "types") {
					MIME::Parser<MIMEParserException>(args[0], this->m_Mime_types);
				}
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::DEFAULT_TYPE: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Default Type arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args[0], "invalid number of Default Type arguments!") : this->m_Default_type = args[0];
			}
			return false;
		}
		case CONF::E_HTTP_BLOCK_STATUS::SERVER: {
			if (args.size() > 1 || (args.size() == 1 && !args[0].empty())) {
				throw ConfParserException(args.at(0), "invalid number of Server arguments!");
			}
			return true;
		}
	}
	throw ConfParserException("", "Invalid configure file!");
}

const std::string	CONF::HTTPBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	std::string	argument;
	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		handleHtabSpace(fileContent.at(Pos[E_INDEX::FILE]));
		Pos[E_INDEX::FILE]++;
	}

	switch (status) {
		case CONF::E_HTTP_BLOCK_STATUS::ROOT:
		case CONF::E_HTTP_BLOCK_STATUS::ACCESS_LOG:
		case CONF::E_HTTP_BLOCK_STATUS::INDEX:
		case CONF::E_HTTP_BLOCK_STATUS::INCLUDE:
			return (stringPathArgumentParser(argument) ? argument : throw ConfParserException(argument, "invalid root argument format!"));
		case CONF::E_HTTP_BLOCK_STATUS::ERROR_PAGE: {
			errorPageArgumentParser(argument);
			return (argument);
		}
		case CONF::E_HTTP_BLOCK_STATUS::DEFAULT_TYPE: {
			const std::size_t	startPos = Pos[E_INDEX::FILE];
			argument = MIME::type<ConfParserException>(fileContent, Pos[E_INDEX::FILE]);
			Pos[E_INDEX::COLUMN] += Pos[E_INDEX::FILE] - startPos;
			return (argument);
		}
		case CONF::E_HTTP_BLOCK_STATUS::KEEPALIVE_TIMEOUT: {
			if (!digitArgumentParser(argument)) {
				throw ConfParserException(argument, "invalid number of Keepalive Timeout arguments!");
			}
			return (argument);
		}
	}
	argumentParser(argument);
	return (argument);
}


unsigned short	CONF::HTTPBlock::directiveNameChecker(const std::string& name) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const statusMap::iterator	it = m_HTTPStatusMap.find(name);

	if (it == m_HTTPStatusMap.end()) {
		throw ConfParserException(name, "HTTP directive name is invalid!");
	} else {
		((m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), m_Status)) ? throw ConfParserException(name, "main directive is duplicated!") : m_Status |= it->second;
		return it->second;
	}
}

bool	CONF::HTTPBlock::blockContent() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();


	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException("{", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;

	ft::shared_ptr<CONF::ServerBlock>	server(new ServerBlock(this->m_Autoindex,
												this->m_KeepAliveTime,
												this->m_Root,
												this->m_Access_log,
												this->m_Error_page,
												this->m_Index));

	server->initialize();
	const std::set<std::string>&	nameSet = server->getServerNames();

	for (std::set<std::string>::const_iterator it = nameSet.begin(); it != nameSet.end(); ++it) {
		std::cout << BOLDRED << *it << std::endl;
		const serverKey	toFind = std::make_pair(*it, server->getPort());

		if (this->m_Server_block.find(toFind) != this->m_Server_block.end()) {
			std::cerr << "duplicated\n";
		} else {
			this->m_Server_block.insert(std::make_pair(toFind, server));
		}
	}

	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException("}", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}


bool	CONF::HTTPBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

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

void	CONF::HTTPBlock::initialize() {
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::HTTP);
	initHTTPStatusMap();

	contextLines();
}

const ft::shared_ptr<CONF::ServerBlock>&	CONF::HTTPBlock::operator[](const serverKey& key) const {
	serverMap::const_iterator it = m_Server_block.find(key);
	return (it != m_Server_block.end()) ? it->second : throw ConfParserException(key.first, "cannot find server!");
}

const bool&	CONF::HTTPBlock::getAutoindex() const {
	return (this->m_Autoindex);
}

const std::string&	CONF::HTTPBlock::getRoot() const {
	return (this->m_Root);
}

const std::string	CONF::HTTPBlock::getIndex(const std::string& uri) const {
	return (this->m_Index.find(uri));
}

const std::string&	CONF::HTTPBlock::getAccess_log() const {
	return (this->m_Access_log);
}

const unsigned int&	CONF::HTTPBlock::getKeepAliveTime() const {
	return (this->m_KeepAliveTime);
}

const std::string&	CONF::HTTPBlock::getDefault_type() const {
	return (this->m_Default_type);
}

const std::map<unsigned short, CONF::errorPageData>&	CONF::HTTPBlock::getError_page() const {
	return (this->m_Error_page);
}

const CONF::HTTPBlock::TypeMap&	CONF::HTTPBlock::getMime_types() const {
	return (this->m_Mime_types);
}

const std::map<std::pair<std::string, unsigned short>, ft::shared_ptr<CONF::ServerBlock> >	CONF::HTTPBlock::getServerMap() const {
	return (this->m_Server_block);
}
