#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	RequestEntityTooLongException : std::exception {
	private:
		std::string		m_Message;

	public:
		RequestEntityTooLongException();
		RequestEntityTooLongException(const RequestEntityTooLongException& other);
		RequestEntityTooLongException& operator=(const RequestEntityTooLongException& other);
		virtual ~RequestEntityTooLongException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}
