#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	RequestURITooLongException : std::exception {
	private:

	public:
		RequestURITooLongException();
		RequestURITooLongException(const RequestURITooLongException& other);
		RequestURITooLongException& operator=(const RequestURITooLongException& other);
		virtual ~RequestURITooLongException() throw();

		virtual const char*	what() const throw();
	};
}
