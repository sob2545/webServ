#include "BadRequestException.hpp"
#include <sstream>
#include <string>

HTTP::BadRequestException::BadRequestException() {}

HTTP::BadRequestException::BadRequestException(const BadRequestException& other) {
	*this = other;
}

HTTP::BadRequestException& HTTP::BadRequestException::operator=(const BadRequestException& other) {
	return *this;
}

HTTP::BadRequestException::~BadRequestException() throw() {}

const char* HTTP::BadRequestException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::BadRequestException::getMessage() const {
	return m_Message;
}