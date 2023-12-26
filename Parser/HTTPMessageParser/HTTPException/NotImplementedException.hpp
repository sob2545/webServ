#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	NotImplementedException : std::exception {
	private:

	public:
		NotImplementedException();
		NotImplementedException(const NotImplementedException& other);
		NotImplementedException& operator=(const NotImplementedException& other);
		virtual ~NotImplementedException() throw();

		virtual const char*	what() const throw();
	};
}
