#pragma once

#include <exception>
#include <string>
#include "../../../utils/Color.hpp"

class URIParserException : public std::exception {
private:
	std::string		m_Message;
	URIParserException();

public:
	URIParserException(const URIParserException& other);
	URIParserException& operator=(const URIParserException& other);
	URIParserException(const std::string& error, const std::string& message);
	virtual ~URIParserException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};