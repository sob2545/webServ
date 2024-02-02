#include "HTTPRequestParsingException.hpp"
#include <sstream>
#include <string>

HTTP::HTTPRequestParsingException::HTTPRequestParsingException(const std::string& error, const std::string& message) {
	std::stringstream	res;
	res << BOLDRED << error << " Error: " << BOLDWHITE << message << RESET;
	m_Message = res.str();
}

HTTP::HTTPRequestParsingException::HTTPRequestParsingException(const HTTPRequestParsingException& other) {
	*this = other;
}

HTTP::HTTPRequestParsingException& HTTP::HTTPRequestParsingException::operator=(const HTTPRequestParsingException& other) {
	return *this;
}

HTTP::HTTPRequestParsingException::~HTTPRequestParsingException() throw() {}

const char* HTTP::HTTPRequestParsingException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::HTTPRequestParsingException::getMessage() const {
	return m_Message;
}