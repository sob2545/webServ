#include "KeepReadHeaderException.hpp"
#include <sstream>
#include <string>

HTTP::KeepReadHeaderException::KeepReadHeaderException() {}

HTTP::KeepReadHeaderException::KeepReadHeaderException(const KeepReadHeaderException& other) {
	*this = other;
}

HTTP::KeepReadHeaderException& HTTP::KeepReadHeaderException::operator=(const KeepReadHeaderException& other) {
	return *this;
}

HTTP::KeepReadHeaderException::~KeepReadHeaderException() throw() {}

const char* HTTP::KeepReadHeaderException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::KeepReadHeaderException::getMessage() const {
	return m_Message;
}