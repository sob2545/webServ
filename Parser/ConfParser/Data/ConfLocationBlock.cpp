#include "ConfLocationBlock.hpp"

// TODO: delete
#include <iostream>
#include <sys/_types/_size_t.h>

std::map<std::string, unsigned short>	CONF::LocationBlock::m_LocationStatusMap;

CONF::LocationBlock::LocationBlock(
	const bool&			autoIndex,
	const std::string&	root,
	const std::string&	accessLog,
	const errorPageMap&	errorPage,
	const Trie&			index
)
: AConfParser(),
  m_Autoindex(autoIndex),
  m_Status(0),
  m_Root(root),
  m_Error_page(errorPage),
  m_Access_log(accessLog),
  m_Index(index)
{}

CONF::LocationBlock::~LocationBlock() {}

void	CONF::LocationBlock::initLocationStatusMap() {
	m_LocationStatusMap["root"] = E_LOCATION_BLOCK_STATUS::ROOT;
	m_LocationStatusMap["index"] = E_LOCATION_BLOCK_STATUS::INDEX;
	m_LocationStatusMap["autoindex"] = E_LOCATION_BLOCK_STATUS::AUTOINDEX;
	m_LocationStatusMap["error_page"] = E_LOCATION_BLOCK_STATUS::ERROR_PAGE;
	m_LocationStatusMap["access_log"] = E_LOCATION_BLOCK_STATUS::ACCESS_LOG;
	m_LocationStatusMap["cgi"] = E_LOCATION_BLOCK_STATUS::CGI;
	m_LocationStatusMap["location"] = E_LOCATION_BLOCK_STATUS::LOCATION;
}


bool	CONF::LocationBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*		Pos = CONF::ConfFile::getInstance()->Pos();

	switch (status) {
		case CONF::E_LOCATION_BLOCK_STATUS::ROOT: {
			if (args.size() != 1) {
				throw ConfParserException(args.at(0), "invalid number of Root arguments!");
			} else {
				(args[0].empty()) ? throw ConfParserException(args.at(0), "invalid number of Root arguments!") : this->m_Root = args[0];
			}
			return false;
		}
		case CONF::E_LOCATION_BLOCK_STATUS::INDEX: {
			args.empty() ? throw ConfParserException("", "index argument is empty!") : 0;
			for (std::size_t i = 0; i < args.size(); i++) {
				(args[i].empty()) ? throw ConfParserException(args.at(0), "invalid number of Index arguments!") : this->m_Index.insert(args[i]);
			}
			return false;
		}
		case CONF::E_LOCATION_BLOCK_STATUS::AUTOINDEX: {
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
		case CONF::E_LOCATION_BLOCK_STATUS::ERROR_PAGE: {
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
		case CONF::E_LOCATION_BLOCK_STATUS::CGI: {
			args.empty() ? throw ConfParserException("", "server_name argument is empty!") : 0;
			// TODO: implement
			return false;
		}
		case CONF::E_LOCATION_BLOCK_STATUS::LOCATION: {
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

const std::string	CONF::LocationBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	std::string	argument;
	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		handleHtabSpace(fileContent.at(Pos[E_INDEX::FILE]));
		Pos[E_INDEX::FILE]++;
	}

	switch (status) {
		case CONF::E_LOCATION_BLOCK_STATUS::ROOT:
		case CONF::E_LOCATION_BLOCK_STATUS::INDEX:
		case CONF::E_LOCATION_BLOCK_STATUS::ACCESS_LOG:
			return (stringPathArgumentParser(argument) ? argument : throw ConfParserException(argument, "invalid root argument format!"));
		case CONF::E_LOCATION_BLOCK_STATUS::ERROR_PAGE: {
			errorPageArgumentParser(argument);
			return (argument);
		}
		case CONF::E_LOCATION_BLOCK_STATUS::LOCATION: {
			const std::size_t	startPos = Pos[E_INDEX::FILE];
			(PathParser::File_AbsolutePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)
					|| PathParser::File_RelativePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument))
				? 0 : throw ConfParserException(argument, "is invalid location directive argument");
			Pos[E_INDEX::COLUMN] += (Pos[E_INDEX::FILE] - startPos);
		}
		case CONF::E_LOCATION_BLOCK_STATUS::CGI: {
			// TODO: cgi는 실행파일의 경로인가?
		}
	}
	argumentParser(argument);
	return (argument);
}


unsigned short	CONF::LocationBlock::directiveNameChecker(const std::string& name) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const statusMap::iterator	it = m_LocationStatusMap.find(name);

	if (it == m_LocationStatusMap.end()) {
		throw ConfParserException(name, "Server directive name is invalid!");
	} else {
		((m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), m_Status)) ? throw ConfParserException(name, "Server directive is duplicated!") : m_Status |= it->second;
		return it->second;
	}
}

bool	CONF::LocationBlock::blockContent() {
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


bool	CONF::LocationBlock::context() {
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

void	CONF::LocationBlock::initialize() {
	this->m_LocationStatusMap.empty() ? initLocationStatusMap() : static_cast<void>(0);
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::SERVER);

	contextLines();
}



const CONF::LocationBlock&	CONF::LocationBlock::operator[](const std::string& locationName) const {
	const locationMap::const_iterator	it = this->m_LocationBlock.find(locationName);

	if (it == this->m_LocationBlock.end()) {
		throw ConfParserException(locationName, "Location block is not found!");
	}
	return (it->second);
}

const bool&	CONF::LocationBlock::getAutoindex() const {
	return (this->m_Autoindex);
}

const std::string&	CONF::LocationBlock::getRoot() const {
	return (this->m_Root);
}

const std::string	CONF::LocationBlock::getIndex(const std::string& uri) const {
	return (this->m_Index.find(uri));
}

const std::string&	CONF::LocationBlock::getAccess_log() const {
	return (this->m_Access_log);
}

const std::string&	CONF::LocationBlock::getCgi() const {
	return (this->m_Cgi);
}

const std::map<unsigned short, CONF::errorPageData>&	CONF::LocationBlock::getError_page() const {
	return (this->m_Error_page);
}

const CONF::LocationBlock::locationMap&	CONF::LocationBlock::getLocationBlock() const {
	return (this->m_LocationBlock);
}