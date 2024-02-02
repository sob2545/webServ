#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	HTTPVersionNotSupportException : std::exception {
	private:
		std::string		m_Message;
		
	public:
		HTTPVersionNotSupportException();
		HTTPVersionNotSupportException(const HTTPVersionNotSupportException& other);
		HTTPVersionNotSupportException& operator=(const HTTPVersionNotSupportException& other);
		virtual ~HTTPVersionNotSupportException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}
