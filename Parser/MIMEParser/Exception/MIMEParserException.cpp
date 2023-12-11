#include "MIMEParserException.hpp"
#include "../MIMEFile/MIMEFile.hpp"
#include <sstream>
#include <string>

MIMEParserException::MIMEParserException() {}

MIMEParserException::MIMEParserException(const std::string& error, const std::string& message) {
	std::stringstream	res;
	if (!MIME::MIMEFile::getInstance()) {
		res << BOLDRED << "Error: " << BOLDWHITE "MIMEParserException: MIMEFile::getInstance() is NULL" << RESET;
		m_Message = res.str();
		return ;
	}
	const std::string&	file = MIME::MIMEFile::getInstance()->getFileName();
	const std::size_t*		pos = MIME::MIMEFile::getInstance()->Pos();

	res << BOLDWHITE << file << ":" << pos[1] << ":" << pos[2] << BOLDRED << " Error:" << BOLDWHITE << " \"" << error << "\" " << message << RESET;
	m_Message = res.str();
}

MIMEParserException::MIMEParserException(const MIMEParserException& other) {
	*this = other;
}

MIMEParserException& MIMEParserException::operator=(const MIMEParserException& other) {
	m_Message = other.m_Message;
	return *this;
}

MIMEParserException::~MIMEParserException() throw() {}

const char* MIMEParserException::what() const throw() {
	return m_Message.c_str();
}

const std::string& MIMEParserException::getMessage() const {
	return m_Message;
}