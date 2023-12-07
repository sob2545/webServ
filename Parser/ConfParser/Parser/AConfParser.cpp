#include "AConfParser.hpp"
#include "Exception/ConfParserException.hpp"
#include <string>

// TODO: delete
#include <iostream>

std::stack<unsigned char> CONF::AConfParser::m_BlockStack;

CONF::AConfParser::AConfParser() {}

CONF::AConfParser::~AConfParser() {}

/**
 *		Common Util Parser
*/
const bool	CONF::AConfParser::isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status) {
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

const bool	CONF::AConfParser::fileName(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*			Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && (std::isalnum(static_cast<int>(fileContent[Pos[E_INDEX::FILE]])) || fileContent[Pos[E_INDEX::FILE]] == '_' || fileContent[Pos[E_INDEX::FILE]] == '.')) {
		argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	if (argument.empty() || !std::isalpha(static_cast<int>((argument.rfind('.') + 1)))) {
		return false;
	}
	return true;
}

/*			absPath with string vector
void	CONF::AConfParser::absPathArgumentParser(strVec& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*			Pos = CONF::ConfFile::getInstance()->Pos();

	const size_t	startPos = Pos[E_INDEX::FILE];
	URIParser::relative(fileContent, Pos[E_INDEX::FILE], argument);
	Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startPos);
}
*/

// 		Does absPath function need?
void	CONF::AConfParser::absPathArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*			Pos = CONF::ConfFile::getInstance()->Pos();

	const size_t	startPos = Pos[E_INDEX::FILE];
	PathParser::File_AbsolutePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument);
	Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startPos);
}

void	CONF::AConfParser::stringPathArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const size_t	startPos = Pos[E_INDEX::FILE];
	if (PathParser::File_AbsolutePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)
			|| PathParser::File_RelativePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)) {
		Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startPos);
	}
}


void	CONF::AConfParser::digitArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && std::isdigit(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) {
		argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	if (!ABNF::isWSP(fileContent, Pos[E_INDEX::FILE]) && fileContent[Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON && fileContent[Pos[E_INDEX::FILE]] != E_ABNF::LF) {
		throw ConfParserException(argument, "invalid type of Worker Connections arguments!");
	}
}

void	CONF::AConfParser::errorPageArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && (std::isalnum(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))
													|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_RESERVED::SLASH
													|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_RESERVED::COLON
													|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_MARK::UNDERSCORE
													|| fileContent[Pos[E_INDEX::FILE]] == BNF::E_MARK::PERIOD))
	{
		argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
}


/**
 *			Common Config Parsing functions
*/
const unsigned short	CONF::AConfParser::directiveName() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*			Pos = CONF::ConfFile::getInstance()->Pos();
	std::string	name;

	while (Pos[E_INDEX::FILE] < fileSize
				&& (std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))
					|| fileContent[Pos[E_INDEX::FILE]] == '_')) {
		(std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) ? name += std::tolower(static_cast<int>(fileContent[Pos[E_INDEX::FILE]])) : name += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	return (directiveNameChecker(name));
}

const bool	CONF::AConfParser::directives() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*			Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_CONF::RBRACE) {
		return (false);
	}
	const unsigned short&		status = directiveName();

	std::vector<std::string>	args;
	while (Pos[E_INDEX::FILE] < fileSize
			&& fileContent[Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON
			&& fileContent[Pos[E_INDEX::FILE]] != E_ABNF::LF
			&& fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		args.push_back(argument(status));
	}
	return (argumentChecker(args, status));
}

const bool	CONF::AConfParser::contextLines() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize) {
		while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
			Pos[E_INDEX::FILE]++;
			Pos[E_INDEX::COLUMN]++;
		}
		if (context() || ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE])) {
			Pos[E_INDEX::LINE]++;
			Pos[E_INDEX::COLUMN] = 1;
		} else {
			return (false);
		}
	}
	return true;
}