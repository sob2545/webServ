#pragma once

#include <exception>
#include <string>

namespace HTTP {
	class	MethodNotFoundException : std::exception {
	private:
		std::string		m_Message;

	public:
		MethodNotFoundException();
		MethodNotFoundException(const MethodNotFoundException& other);
		MethodNotFoundException& operator=(const MethodNotFoundException& other);
		virtual ~MethodNotFoundException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}
