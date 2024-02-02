#pragma once

#include <exception>
#include <string>
#include "../../../Utils/Color.hpp"

namespace HTTP {
	class HTTPRequestParsingException : public std::exception {
private:
	HTTPRequestParsingException();
	std::string		m_Message;

public:
	HTTPRequestParsingException(const HTTPRequestParsingException& other);
	HTTPRequestParsingException& operator=(const HTTPRequestParsingException& other);
	HTTPRequestParsingException(const std::string& error, const std::string& message);
	virtual ~HTTPRequestParsingException() throw();
	virtual const char* what() const throw();
	const std::string& getMessage() const;
	};
}
