#include "HTTPVersionNotSupportException.hpp"
#include <sstream>
#include <string>

HTTP::HTTPVersionNotSupportException::HTTPVersionNotSupportException() {}

HTTP::HTTPVersionNotSupportException::HTTPVersionNotSupportException(const HTTPVersionNotSupportException& other) {
	*this = other;
}

HTTP::HTTPVersionNotSupportException& HTTP::HTTPVersionNotSupportException::operator=(const HTTPVersionNotSupportException& other) {
	return *this;
}

HTTP::HTTPVersionNotSupportException::~HTTPVersionNotSupportException() throw() {}

const char* HTTP::HTTPVersionNotSupportException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::HTTPVersionNotSupportException::getMessage() const {
	return m_Message;
}