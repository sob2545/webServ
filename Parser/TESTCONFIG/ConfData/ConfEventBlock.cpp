#include "ConfEventBlock.hpp"
#include <string>

// TODO: delete
#include <iostream>

CONF::EventsBlock::EventsBlock(): m_Status(false), m_Worker_connections(1024) {}

CONF::EventsBlock::~EventsBlock() {}


const bool	CONF::EventsBlock::argumentChecker(const std::vector<std::string>& args, const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const std::string&	fileName = CONF::ConfFile::getInstance()->getFileName();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	if (args.size() != 1) {
		throw ConfParserException(fileName, args.at(0), "invalid number of Events arguments!", Pos);
	} else {
		if (args.at(0).empty()) {
			throw ConfParserException(fileName, args.at(0), "invalid number of Events arguments!", Pos);
		}
		char*	endptr;
		const long	argumentNumber = std::strtol(args[0].c_str(), &endptr, 10);
		if (*endptr != '\0' || argumentNumber < 1) {
			throw ConfParserException(fileName, args[0], "invalid number of Events arguments!", Pos);
		}
		this->m_Worker_connections = static_cast<unsigned int>(argumentNumber);
	}
	return (false);
}

const std::string	CONF::EventsBlock::argument(const unsigned short& status) {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	while (Pos[E_INDEX::FILE] < fileSize && ABNF::isWSP(fileContent, Pos[E_INDEX::FILE])) {
		Pos[E_INDEX::FILE]++;
		Pos[E_INDEX::COLUMN]++;
	}
	std::string			argument;

	if (status == E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS) {
		digitArgumentParser(argument);
		std::cout << "worker_connections: " << argument << std::endl;
	} else {
		throw ConfParserException(CONF::ConfFile::getInstance()->getFileName(), "", "is invalid Confgiure file!", Pos);
	}
	return (argument);
}

const unsigned short	CONF::EventsBlock::directiveNameChecker(const std::string& name) {
	if (name == "worker_connections") {
		(m_Status & E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS) ? throw ConfParserException(CONF::ConfFile::getInstance()->getFileName(), name, "events directive is duplicated!", CONF::ConfFile::getInstance()->Pos()) : m_Status |= E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS;
		return (E_EVENTS_BLOCK_STATUS::WORKER_CONNECTIONS);
	} else {
		throw ConfParserException(CONF::ConfFile::getInstance()->getFileName(), name, "events directive name is invalid!", CONF::ConfFile::getInstance()->Pos());
	}
}

const bool	CONF::EventsBlock::context() {
	const std::string&	fileContent = CONF::ConfFile::getInstance()->getFileContent();
	const size_t&		fileSize = CONF::ConfFile::getInstance()->getFileSize();
	size_t*				Pos = CONF::ConfFile::getInstance()->Pos();

	if (fileContent[Pos[E_INDEX::FILE]] == E_ABNF::SEMICOLON
			|| fileContent[Pos[E_INDEX::FILE]] == E_ABNF::LF) {
		return (false);
	}
	if (directives()) {
		throw ConfParserException(CONF::ConfFile::getInstance()->getFileName(), "", "is invalid Confgiure file!", Pos);
	} else {
		return (ABNF::isC_nl(fileContent, Pos[E_INDEX::FILE]) ? true : false);
	}
}

void	CONF::EventsBlock::initialize() {
	CONF::AConfParser::m_BlockStack.push(CONF::E_BLOCK_STATUS::EVENT);

	contextLines();
}