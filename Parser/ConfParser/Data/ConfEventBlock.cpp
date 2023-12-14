#include "ConfEventBlock.hpp"
#include <string>

// TODO: delete
#include <iostream>

CONF::EventsBlock::EventsBlock(): m_Status(false), m_Worker_connections(1024) {}

CONF::EventsBlock::~EventsBlock() {}


bool	CONF::EventsBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	static_cast<void>(status);

	if (args.size() != 1) {
		throw ConfParserException(args.at(0), "invalid number of Events arguments!");
	} else {
		if (args.at(0).empty()) {
			throw ConfParserException(args.at(0), "invalid number of Events arguments!");
		}
		char*	endptr;
		const long	argumentNumber = std::strtol(args[0].c_str(), &endptr, 10);
		if (*endptr != '\0' || argumentNumber < 1) {
			throw ConfParserException(args[0], "invalid worker_connections argument!");
		}
		this->m_Worker_connections = static_cast<unsigned int>(argumentNumber);
	}
	return (false);
}

const std::string	CONF::EventsBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		handleHtabSpace(fileContent.at(Pos[E_INDEX::FILE]));
		Pos[E_INDEX::FILE]++;
	}
	std::string			argument;

	if (status == E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS) {
		// digitArgumentParser(argument);
		// TODO : comment나 nl 또는 WSP가 아닐 때까지 파싱
		while (Pos[E_INDEX::FILE] < fileSize
			&& (std::isalnum(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))
				|| fileContent[Pos[E_INDEX::FILE]] == '_'
				|| fileContent[Pos[E_INDEX::FILE]] == '=')) {
			(std::isalpha(static_cast<int>(fileContent[Pos[E_INDEX::FILE]]))) ? argument += std::tolower(fileContent[Pos[E_INDEX::FILE]]) : argument += fileContent[Pos[E_INDEX::FILE]];
			Pos[E_INDEX::FILE]++;
			Pos[E_INDEX::COLUMN]++;
		}
		std::cout << "worker_connections: " << argument << std::endl;
	} else {
		throw ConfParserException("", "is invalid Confgiure file!");
	}
	return (argument);
}

unsigned short	CONF::EventsBlock::directiveNameChecker(const std::string& name) {
	if (name == "worker_connections") {
		(m_Status & E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS) ? throw ConfParserException(name, "events directive is duplicated!") : m_Status |= E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS;
		return (E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS);
	} else {
		throw ConfParserException(name, "events directive name is invalid!");
	}
}

bool	CONF::EventsBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	std::size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| fileContent[Pos[E_INDEX::FILE]] == E_ABNF::LF) {
		return (false);
	}
	if (directives()) {
		throw ConfParserException("", "is invalid Confgiure file!");
	} else {
		return (ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]) ? true : false);
	}
}

void	CONF::EventsBlock::initialize() {
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::EVENT);

	contextLines();
}