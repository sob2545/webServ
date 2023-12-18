#pragma once

#include <exception>
#include <string>
#include "../../../../Utils/Color.hpp"

class EnvParserException : public std::exception {
private:
	std::string		m_Message;
	EnvParserException();

public:
	EnvParserException(const EnvParserException& other);
	EnvParserException& operator=(const EnvParserException& other);
	EnvParserException(const std::string& error, const std::string& message);
	virtual ~EnvParserException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
};