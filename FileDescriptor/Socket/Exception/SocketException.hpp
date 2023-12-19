#pragma once

#include <string>
#include <exception>

namespace SOCK {
	class SocketException : public std::exception {
	private:
		std::string	m_Message;

	public:
		explicit SocketException(const std::string& message) throw();
		SocketException& operator=(const SocketException& other);
		SocketException(const SocketException& other);
		~SocketException() throw();

		virtual const char* what() const throw();
		const std::string& getMessage() const;
	};
}