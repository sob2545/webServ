#pragma once

#include <exception>
#include <string>
#include "../../../Utils/Color.hpp"

class MIMEParserException : public std::exception {
private:
	std::string		m_Message;
	MIMEParserException();

public:
	MIMEParserException(const MIMEParserException& other);
	MIMEParserException& operator=(const MIMEParserException& other);
	MIMEParserException(const std::string& error, const std::string& message);
	virtual ~MIMEParserException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};