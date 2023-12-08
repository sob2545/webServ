#include "ConfHTTPBlock.hpp"
#include "ConfServerBlock.hpp"
#include <sys/_types/_size_t.h>

CONF::HTTPBlock::HTTPBlock()
: AConfParser(),
  m_Autoindex(false),
  m_Status(0),
  m_Default_type("text/plain") {
	initHTTPStatusMap();
}

CONF::HTTPBlock::~HTTPBlock() {}

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


const bool	CONF::HTTPBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {

}

const std::string	CONF::HTTPBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	std::string	argument;
	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}

	switch (status) {
		case CONF::E_HTTP_BLOCK_STATUS::ROOT:
		case CONF::E_HTTP_BLOCK_STATUS::ACCESS_LOG:
		case CONF::E_HTTP_BLOCK_STATUS::INDEX:
		case CONF::E_HTTP_BLOCK_STATUS::INCLUDE:
			stringPathArgumentParser(argument);
			return (argument);
		case CONF::E_HTTP_BLOCK_STATUS::ERROR_PAGE: {
			errorPageArgumentParser(argument);
			return (argument);
		}
	}
	// 기본적인 파싱 : only alnum 
	while (Pos[E_INDEX::FILE] < fileSize
			&& (std::isalnum(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))
				|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_MARK::UNDERSCORE
				|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_MARK::HYPHEN
				|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_MARK::PERIOD
				|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_RESERVED::SLASH
				|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_RESERVED::PLUS)) {
		(std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) ? argument += std::tolower(fileContent[Pos[E_INDEX::FILE]]) : argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	return (argument);
}


const unsigned short	CONF::HTTPBlock::directiveNameChecker(const std::string& name) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const statusMap::iterator	it = m_HTTPStatusMap.find(name);

	if (it == m_HTTPStatusMap.end()) {
		throw ConfParserException(name, "HTTP directive name is invalid!");
	} else {
		((m_Status & it->second) && !isMultipleDirective(m_BlockStack.top(), m_Status)) ? throw ConfParserException(name, "main directive is duplicated!") : m_Status |= it->second;
		return it->second;
	}
}

const bool	CONF::HTTPBlock::blockContent() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();


	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		throw ConfParserException("{", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;

	// TODO: implement serverBlock
	ServerBlock	serverBlock;
	serverBlock.initialize();

	m_Server_block.insert(std::make_pair(serverBlock.getServerName(), serverBlock));

	if (fileContent[Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		throw ConfParserException("}", "Direct block has no brace!");
	}
	Pos[E_INDEX::FILE]++;
	Pos[E_INDEX::COLUMN]++;
	m_BlockStack.pop();
	return true;
}


const bool	CONF::HTTPBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
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

void	CONF::HTTPBlock::initialize() {
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::HTTP);

	contextLines();
}

