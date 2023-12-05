#pragma once

#include <exception>
#include <string>
#include "../../../../utils/Color.hpp"

class ConfParserException : public std::exception {
private:
	std::string		m_Message;
	ConfParserException();

public:
	ConfParserException(const ConfParserException& other);
	ConfParserException& operator=(const ConfParserException& other);
	ConfParserException(const std::string& file, const std::string& error, const std::string& message, const size_t* pos);
	virtual ~ConfParserException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};