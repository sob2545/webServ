#include "ConfParserException.hpp"
#include "../../File/ConfFile.hpp"
#include <sstream>
#include <string>

ConfParserException::ConfParserException() {}

ConfParserException::ConfParserException(const std::string& error, const std::string& message) {
	std::stringstream	res;
	if (!CONF::ConfFile::getInstance()) {
		res << BOLDRED << "Error: " << BOLDWHITE "ConfParserException: ConfFile::getInstance() is NULL" << RESET;
		m_Message = res.str();
		return ;
	}
	const std::string&	file = CONF::ConfFile::getInstance()->getFileName();
	const size_t*		pos = CONF::ConfFile::getInstance()->Pos();

	res << BOLDWHITE << file << ":" << pos[1] << ":" << pos[2] << BOLDRED << " Error:" << BOLDWHITE << " \"" << error << "\" " << message << RESET;
	m_Message = res.str();
}

ConfParserException::ConfParserException(const ConfParserException& other) {
	*this = other;
}

ConfParserException& ConfParserException::operator=(const ConfParserException& other) {
	m_Message = other.m_Message;
	return *this;
}

ConfParserException::~ConfParserException() throw() {}

const char* ConfParserException::what() const throw() {
	return m_Message.c_str();
}

const std::string& ConfParserException::getMessage() const {
	return m_Message;
}