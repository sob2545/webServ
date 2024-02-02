#include "ClientCloseException.hpp"

ClientCloseException::ClientCloseException() throw()
{}

ClientCloseException& ClientCloseException::operator=(const ClientCloseException& other) {
  m_Message = other.m_Message;
  return (*this);
}

ClientCloseException::ClientCloseException(const ClientCloseException& other) {
  *this = other;
}

ClientCloseException::~ClientCloseException() throw() {}

const char* ClientCloseException::what() const throw() {
  return (m_Message.c_str());
}

const std::string&  ClientCloseException::getMessage() const {
  return (m_Message);
}