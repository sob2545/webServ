#include "SocketException.hpp"

SOCK::SocketException::SocketException(const std::string& message) throw()
  : m_Message(message)
{}

SOCK::SocketException& SOCK::SocketException::operator=(const SocketException& other) {
  m_Message = other.m_Message;
  return (*this);
}

SOCK::SocketException::SocketException(const SocketException& other) {
  *this = other;
}

SOCK::SocketException::~SocketException() throw() {}

const char* SOCK::SocketException::what() const throw() {
  return (m_Message.c_str());
}

const std::string&  SOCK::SocketException::getMessage() const {
  return (m_Message);
}