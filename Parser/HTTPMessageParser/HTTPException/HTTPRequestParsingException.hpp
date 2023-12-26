#pragma once

#include <exception>
#include <string>
#include "../../../Utils/Color.hpp"

class HTTPRequestParsingException : public std::exception {
private:
	std::string		m_Message;
	HTTPRequestParsingException();

public:
	HTTPRequestParsingException(const HTTPRequestParsingException& other);
	HTTPRequestParsingException& operator=(const HTTPRequestParsingException& other);
	HTTPRequestParsingException(const std::string& error, const std::string& message);
	virtual ~HTTPRequestParsingException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};