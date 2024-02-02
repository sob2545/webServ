#include "RequestURITooLongException.hpp"
#include <sstream>
#include <string>

HTTP::RequestURITooLongException::RequestURITooLongException() {}

HTTP::RequestURITooLongException::RequestURITooLongException(const RequestURITooLongException& other) {
	*this = other;
}

HTTP::RequestURITooLongException& HTTP::RequestURITooLongException::operator=(const RequestURITooLongException& other) {
	return *this;
}

HTTP::RequestURITooLongException::~RequestURITooLongException() throw() {}

const char* HTTP::RequestURITooLongException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::RequestURITooLongException::getMessage() const {
	return m_Message;
}