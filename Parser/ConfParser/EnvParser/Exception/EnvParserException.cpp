#include "EnvParserException.hpp"
#include <sstream>
#include <string>

EnvParserException::EnvParserException() {}

EnvParserException::EnvParserException(const std::string& error, const std::string& message) {
	std::stringstream	res;

	res << BOLDRED << "Error:" << BOLDWHITE << "\"" << error << "\"" << message << RESET;
	m_Message = res.str();
}

EnvParserException::EnvParserException(const EnvParserException& other) {
	*this = other;
}

EnvParserException& EnvParserException::operator=(const EnvParserException& other) {
	m_Message = other.m_Message;
	return *this;
}

EnvParserException::~EnvParserException() throw() {}

const char* EnvParserException::what() const throw() {
	return m_Message.c_str();
}

const std::string& EnvParserException::getMessage() const {
	return m_Message;
}