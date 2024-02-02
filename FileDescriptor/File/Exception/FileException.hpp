#pragma once

#include <string>
#include <exception>

class FileException : public std::exception {
private:
	std::string	m_Message;

public:
	explicit FileException(const std::string& message) throw();
	FileException& operator=(const FileException& other);
	FileException(const FileException& other);
	~FileException() throw();

	virtual const char* what() const throw();
	const std::string& getMessage() const;
};