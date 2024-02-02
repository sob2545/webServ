#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	BadRequestException : std::exception {
	private:
		std::string		m_Message;

	public:
		BadRequestException();
		BadRequestException(const BadRequestException& other);
		BadRequestException& operator=(const BadRequestException& other);
		virtual ~BadRequestException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}
