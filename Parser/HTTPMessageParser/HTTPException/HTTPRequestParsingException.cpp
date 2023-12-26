#include "HTTPRequestParsingException.hpp"
#include <sstream>
#include <string>

HTTPRequestParsingException::HTTPRequestParsingException() {}

HTTPRequestParsingException::HTTPRequestParsingException(const std::string& error, const std::string& message) {
	this->m_Message = error + message;
}

HTTPRequestParsingException::HTTPRequestParsingException(const HTTPRequestParsingException& other) {
	*this = other;
}

HTTPRequestParsingException& HTTPRequestParsingException::operator=(const HTTPRequestParsingException& other) {
	m_Message = other.m_Message;
	return *this;
}

HTTPRequestParsingException::~HTTPRequestParsingException() throw() {}

const char* HTTPRequestParsingException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTPRequestParsingException::getMessage() const {
	return m_Message;
}