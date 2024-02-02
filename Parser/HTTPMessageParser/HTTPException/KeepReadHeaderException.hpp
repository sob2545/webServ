#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	KeepReadHeaderException : std::exception {
	private:
		std::string		m_Message;
		
	public:
		KeepReadHeaderException();
		KeepReadHeaderException(const KeepReadHeaderException& other);
		KeepReadHeaderException& operator=(const KeepReadHeaderException& other);
		virtual ~KeepReadHeaderException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}
