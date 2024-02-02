#include "NotImplementedException.hpp"
#include <sstream>
#include <string>

HTTP::NotImplementedException::NotImplementedException() {}

HTTP::NotImplementedException::NotImplementedException(const NotImplementedException& other) {
	*this = other;
}

HTTP::NotImplementedException& HTTP::NotImplementedException::operator=(const NotImplementedException& other) {
	return *this;
}

HTTP::NotImplementedException::~NotImplementedException() throw() {}

const char* HTTP::NotImplementedException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::NotImplementedException::getMessage() const {
	return m_Message;
}