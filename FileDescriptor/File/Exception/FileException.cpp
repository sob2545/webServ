#include "FileException.hpp"

FileException::FileException(const std::string& message) throw()
  : m_Message(message)
{}

FileException& FileException::operator=(const FileException& other) {
  m_Message = other.m_Message;
  return (*this);
}

FileException::FileException(const FileException& other) {
  *this = other;
}

FileException::~FileException() throw() {}

const char* FileException::what() const throw() {
  return (m_Message.c_str());
}

const std::string&  FileException::getMessage() const {
  return (m_Message);
}