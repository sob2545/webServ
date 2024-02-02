#include "AConfParser.hpp"
#include "ConfParserUtils.hpp"
#include "Exception/ConfParserException.hpp"
#include <cstddef>
#include <string>

// TODO: delete
#include <iostream>

std::stack<unsigned char> CONF::AConfParser::m_BlockStack;

CONF::AConfParser::AConfParser() {}

CONF::AConfParser::~AConfParser() {}

/**
 *		Common Util Parser
*/
bool	CONF::AConfParser::isMultipleDirective(const unsigned char& block_status, const unsigned short& directive_status) {
	switch (block_status) {
		case CONF::E_BLOCK_STATUS::MAIN: {
			return (directive_status & CONF::E_MAIN_BLOCK_STATUS::ENV) ? true : false;
		}
		case CONF::E_BLOCK_STATUS::HTTP: {
			return ((directive_status & E_HTTP_BLOCK_STATUS::SERVER || directive_status & E_HTTP_BLOCK_STATUS::INCLUDE) ? true : false);
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

void	CONF::AConfParser::fileName(std::string& argument) {
	const std::size_t	lastSlashPos = argument.find_last_of('/');
	argument.find('.', lastSlashPos) != std::string::npos ? throw ConfParserException(argument, "Invalid Path") : 0;
	std::isalpha(static_cast<int>(argument[argument.rfind('.') + 1])) ? 0 : throw ConfParserException(argument, "Invalid Path");
}

/*			absPath with string vector
void	CONF::AConfParser::absPathArgumentParser(StrVec_t& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*			Pos = CONF::ConfFile::getInstance()->Pos();

	const std::size_t	startPos = Pos[E_INDEX::FILE];
	URIParser::relative(fileContent, Pos[E_INDEX::FILE], argument);
	Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startPos);
}
*/


bool	CONF::AConfParser::stringPathArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();
	std::size_t&				file = Pos[E_INDEX::FILE];

	const std::size_t	startPos = Pos[E_INDEX::FILE];
	if (fileContent[Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| ABNF::isLF(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
		return false;
	}
	if (PathParser::File_AbsolutePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)
			|| PathParser::File_RelativePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)) {
		Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startPos);
		return true;
	} else {
		return false;
	}
}

bool	CONF::AConfParser::digitArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	if (!std::isdigit(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) {
		return false;
	}
	while (Pos[E_INDEX::FILE] < fileSize && std::isdigit(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) {
		argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	if (!ABNF::isWSP(fileContent, Pos[E_INDEX::FILE]) && fileContent[Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON && fileContent[Pos[E_INDEX::FILE]] != E_ABNF::LF) {
		throw ConfParserException(argument, "invalid type of Worker Connections arguments!");
	}
	return true;
}

void	CONF::AConfParser::errorPageArgumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	const std::size_t		startFilePos = Pos[E_INDEX::FILE];

	argument.clear();
	(URIParser::errorPageParser<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)
		|| PathParser::File_AbsolutePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)
		|| PathParser::File_RelativePath<ConfParserException>(fileContent, Pos[E_INDEX::FILE], argument)) ? 
	Pos[E_INDEX::COLUMN] += ((Pos[E_INDEX::FILE]) - startFilePos) : throw ConfParserException("", "not path");
}


void	CONF::AConfParser::errorPageChecker(const std::vector<std::string>& args, ErrorPageMap_t& errorMap) {
	const std::size_t	argumentSize = args.size();

	(argumentSize < 2) ? throw ConfParserException("", "invalid number of error page arguments!") : 0;

	(args[argumentSize - 1].empty()) ? throw ConfParserException("", "invalid error page arguments!") : 0;
	const std::string	path = args[argumentSize - 1];

	(args[argumentSize - 2].empty()) ? throw ConfParserException("", "invalid error page arguments!") : 0;
	const char&			startSymbol = args[argumentSize - 2][0];
	CONF::errorPageData	data;
	if (path[0] == '@') {
		data.m_Type = E_ERRORPAGE::LOCATION;
	} else {
		if (std::isdigit(static_cast<int>(startSymbol))) {
			data.m_Type = E_ERRORPAGE::DEFAULT;
		} else if (startSymbol == BNF::E_RESERVED::EQUALS) {
			data.m_Type = E_ERRORPAGE::REPLACE;
			char*	ptr;
			const short replaceCode = strtol(args[argumentSize - 2].c_str() + 1, &ptr, 10);
			(*ptr != '\0' || replaceCode < 0 || replaceCode > 599) ? throw ConfParserException(args[argumentSize - 2], "is invalid error page replace arguments!") : data.m_Replace = replaceCode;
		} else {
			throw ConfParserException(args[argumentSize - 2], "is invalid error page arguments!");
		}
	}
	data.m_Path = path;
	const std::size_t	lastArgument = (data.m_Type > 1) ? 2 : 1;
	char*	endPos;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end() - lastArgument; ++it) {
		endPos = NULL;
		const short	statusCode = strtol(it->c_str(), &endPos, 10);
		(*endPos != '\0' || statusCode < 0 || statusCode > 599) ? throw ConfParserException(*it, "is invalid error page arguments!") : 0;
		errorMap.insert(std::make_pair(statusCode, data));
	}
}

void	CONF::AConfParser::argumentParser(std::string& argument) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*		Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize
			&& !ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])
			&& !ABNF::isLF(fileContent, Pos[E_INDEX::FILE])
			&& fileContent[Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON
			&& fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE
			&& fileContent[Pos[E_INDEX::FILE]] != E_CONF::RBRACE) {
		(std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) ? argument += std::tolower(fileContent[Pos[E_INDEX::FILE]]) : argument += fileContent[Pos[E_INDEX::FILE]];
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
}



/**
 *			Common Config Parsing functions
*/
unsigned short	CONF::AConfParser::directiveName() {
	std::string	name;

	argumentParser(name);
	return (directiveNameChecker(name));
}

bool	CONF::AConfParser::directives() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&	fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*		Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_CONF::RBRACE) {
		return (false);
	}
	const unsigned short&		status = directiveName();

	std::vector<std::string>	args;
	while (Pos[E_INDEX::FILE] < fileSize
			&& fileContent[Pos[E_INDEX::FILE]] != E_ABNF::SEMICOLON
			&& fileContent[Pos[E_INDEX::FILE]] != E_ABNF::LF
			&& fileContent[Pos[E_INDEX::FILE]] != E_CONF::LBRACE) {
		const std::string arg = argument(status);
		if (arg.size()) {
			args.push_back(arg);
		}
	}
	return (argumentChecker(args, status));
}

void	CONF::AConfParser::handleHtabSpace(const char& c) {
	std::size_t&		Pos = CONF::ConfFile::getInstance()->Pos()[E_INDEX::COLUMN];

	(c == E_ABNF::HTAB) ? (Pos += E_CONF::HTAB_SIZE - ((Pos - 1) % E_CONF::HTAB_SIZE)) : Pos++;
}

bool	CONF::AConfParser::contextLines() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize) {
		while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
			handleHtabSpace(fileContent.at(Pos[E_INDEX::FILE]));
			Pos[E_INDEX::FILE]++;
		}
		// (isalpha(fileContent.at(Pos[E_INDEX::FILE]))
		if (context() || ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE])) {
			Pos[E_INDEX::LINE]++;
			Pos[E_INDEX::COLUMN] = 1;
		} else {
			return (false);
		}
	}
	return true;
}