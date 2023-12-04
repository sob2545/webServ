#include "ConfParser.hpp"
#include "Exception/ConfParserException.hpp"
#include <cctype>
#include <string>

CONF::ConfParser::ConfParser() {}

CONF::ConfParser::~ConfParser() {}

/**
 *		Common Util Parser
*/
const std::string&	CONF::ConfParser::fileContent() const {
	return CONF::ConfFile::getInstance()->getFileContent();
}

const std::string&	CONF::ConfParser::fileName() const {
	return CONF::ConfFile::getInstance()->getFileName();
}

size_t*	CONF::ConfParser::Pos() {
	return CONF::ConfFile::getInstance()->Pos();
}

const bool	CONF::ConfParser::fileName(std::string& argument) {
	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize() && (std::isalnum(static_cast<int>(CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]])) || CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == '_' || CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == '.')) {
		argument += CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]];
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]++;
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN]++;
	}
	if (argument.empty() || !std::isalpha(static_cast<int>((argument.rfind('.') + 1)))) {
		return false;
	}
	return true;
}

void	CONF::ConfParser::absPathArgumentParser(strVec& argument) {
	const size_t		argumentLength = (std::strchr(&(CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]]), E_ABNF::LF) - CONF::ConfFile::getInstance()->getFileContent().c_str()) - CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] - 1;
	const std::string	uri = "/" + CONF::ConfFile::getInstance()->getFileContent().substr(CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE], argumentLength);
	size_t	uriPos = 0;
	URIParser::absPath(uri, uriPos, argument);
	CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] += argumentLength + 1;
	CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN] += argumentLength + 1;
}

void	CONF::ConfParser::digitArgumentParser(std::string& argument) {
	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize() && std::isdigit(static_cast<int>(CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]]))) {
		argument += CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]];
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]++;
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN]++;
	}
	if (!ABNF::isWSP(CONF::ConfFile::getInstance()->getFileContent(), CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]) && CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] != E_ABNF::SEMICOLON && CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] != E_ABNF::LF) {
		throw ConfParserException(CONF::ConfFile::getInstance()->getFileName(), argument, "invalid type of Worker Connections arguments!", CONF::ConfFile::getInstance()->Pos());
	}
}

void	CONF::ConfParser::errorPageArgumentParser(std::string& argument) {
	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize() && (std::isalnum(static_cast<int>(CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]]))
													|| CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == URI::E_RESERVED::SLASH
													|| CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == URI::E_RESERVED::COLON
													|| CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == URI::E_MARK::UNDERSCORE
													|| CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == URI::E_MARK::PERIOD))
	{
		argument += CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]];
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]++;
		CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN]++;
	}
}


/**
 *			Common Config Parsing functions
*/
const unsigned short	CONF::ConfParser::directiveName() {
	std::string	name;

	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize()
				&& (std::isalpha(static_cast<int>(CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]]))
					|| CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]] == '_')) {
		(std::isalpha(static_cast<int>(CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]]))) ? name += std::tolower(static_cast<int>(CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]])) : name += CONF::ConfParser::fileContent()[CONF::ConfParser::Pos()[E_INDEX::FILE]];
	}
}

const bool	CONF::ConfParser::directives() {
	if (CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] == E_CONF::RBRACE) {
		return (false);
	}
	const unsigned short&		status = directiveName();

	std::vector<std::string>	args;
	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize()
			&& CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] != E_ABNF::SEMICOLON
			&& CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] != E_ABNF::LF
			&& CONF::ConfFile::getInstance()->getFileContent()[CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]] != E_CONF::LBRACE) {
		args.push_back(argument(status));
	}
	return (argumentChecker(args, status));
}

const bool	CONF::ConfParser::contextLines() {
	while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize()) {
		while (CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE] < CONF::ConfFile::getInstance()->getFileSize() && ABNF::isWSP(CONF::ConfFile::getInstance()->getFileContent(), CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE])) {
			CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE]++;
			CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN]++;
		}
		if (context() || ABNF::isC_nl(CONF::ConfFile::getInstance()->getFileContent(), CONF::ConfFile::getInstance()->Pos()[E_INDEX::FILE])) {
			CONF::ConfFile::getInstance()->Pos()[E_INDEX::LINE]++;
			CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN] = 1;
		} else {
			return (false);
		}
	}
	return true;
}