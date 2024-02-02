#pragma once

#include <string>
#include <exception>

class ClientCloseException : public std::exception {
private:
	std::string	m_Message;

public:
	explicit ClientCloseException() throw();
	ClientCloseException& operator=(const ClientCloseException& other);
	ClientCloseException(const ClientCloseException& other);
	~ClientCloseException() throw();

	virtual const char* what() const throw();
	const std::string& getMessage() const;
};