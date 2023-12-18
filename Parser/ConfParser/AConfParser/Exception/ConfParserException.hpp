#pragma once

#include <exception>
#include <string>
#include "../../../../Utils/Color.hpp"

class ConfParserException : public std::exception {
private:
	std::string		m_Message;
	ConfParserException();

public:
	ConfParserException(const ConfParserException& other);
	ConfParserException& operator=(const ConfParserException& other);
	ConfParserException(const std::string& error, const std::string& message);
	virtual ~ConfParserException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};