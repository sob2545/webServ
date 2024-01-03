#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	LengthRequriedException : std::exception {
	private:

	public:
		LengthRequriedException();
		LengthRequriedException(const LengthRequriedException& other);
		LengthRequriedException& operator=(const LengthRequriedException& other);
		virtual ~LengthRequriedException() throw();

		virtual const char*	what() const throw();
	};
}