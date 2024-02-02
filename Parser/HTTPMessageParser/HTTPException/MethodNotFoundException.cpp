#include "MethodNotFoundException.hpp"
#include <sstream>
#include <string>

HTTP::MethodNotFoundException::MethodNotFoundException() {}

HTTP::MethodNotFoundException::MethodNotFoundException(const MethodNotFoundException& other) {
	*this = other;
}

HTTP::MethodNotFoundException& HTTP::MethodNotFoundException::operator=(const MethodNotFoundException& other) {
	return *this;
}

HTTP::MethodNotFoundException::~MethodNotFoundException() throw() {}

const char* HTTP::MethodNotFoundException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::MethodNotFoundException::getMessage() const {
	return m_Message;
}