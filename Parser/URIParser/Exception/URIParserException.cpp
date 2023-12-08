#include "URIParserException.hpp"
#include <sstream>
#include <string>

URIParserException::URIParserException() {}

URIParserException::URIParserException(const std::string& error, const std::string& message) {
	std::stringstream	res;
	
	res << BOLDRED << "Error: \"" << BOLDWHITE << error << "\" " << message << RESET;
	m_Message = res.str();
}

URIParserException::URIParserException(const URIParserException& other) {
	*this = other;
}

URIParserException& URIParserException::operator=(const URIParserException& other) {
	m_Message = other.m_Message;
	return *this;
}

URIParserException::~URIParserException() throw() {}

const char* URIParserException::what() const throw() {
	return m_Message.c_str();
}

const std::string& URIParserException::getMessage() const {
	return m_Message;
}