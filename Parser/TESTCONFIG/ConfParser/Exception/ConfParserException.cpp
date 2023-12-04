#include "ConfParserException.hpp"
#include <sstream>

ConfParserException::ConfParserException() {}

ConfParserException::ConfParserException(const std::string& file, const std::string& error, const std::string& message, const size_t* pos) {
	std::stringstream	res;

	res << BOLDWHITE << file << ":" << pos[CONF::LINE] << ":" << pos[CONF::COLUMN] << BOLDRED << " Error:" << BOLDWHITE << " \"" << error << "\" " << message << RESET;
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