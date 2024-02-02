#include "LengthRequriedException.hpp"
#include <sstream>
#include <string>

HTTP::LengthRequriedException::LengthRequriedException() {}

HTTP::LengthRequriedException::LengthRequriedException(const LengthRequriedException& other) {
	*this = other;
}

HTTP::LengthRequriedException& HTTP::LengthRequriedException::operator=(const LengthRequriedException& other) {
	return *this;
}

HTTP::LengthRequriedException::~LengthRequriedException() throw() {}

const char* HTTP::LengthRequriedException::what() const throw() {
	return m_Message.c_str();
}

const std::string& HTTP::LengthRequriedException::getMessage() const {
	return m_Message;
}