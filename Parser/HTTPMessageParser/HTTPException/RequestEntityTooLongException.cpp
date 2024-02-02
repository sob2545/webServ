#include "RequestEntityTooLongException.hpp"
#include <sstream>
#include <string>

HTTP::RequestEntityTooLongException::RequestEntityTooLongException() {}

HTTP::RequestEntityTooLongException::RequestEntityTooLongException(const RequestEntityTooLongException& other) {
	*this = other;
}

HTTP::RequestEntityTooLongException& HTTP::RequestEntityTooLongException::operator=(const RequestEntityTooLongException& other) {
	return *this;
}

HTTP::RequestEntityTooLongException::~RequestEntityTooLongException() throw() {}

const char* HTTP::RequestEntityTooLongException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::RequestEntityTooLongException::getMessage() const {
	return m_Message;
}