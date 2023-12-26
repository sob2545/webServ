#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	BadRequestException : std::exception {
	private:

	public:
		BadRequestException();
		BadRequestException(const BadRequestException& other);
		BadRequestException& operator=(const BadRequestException& other);
		virtual ~BadRequestException() throw();

		virtual const char*	what() const throw();
	};
}
