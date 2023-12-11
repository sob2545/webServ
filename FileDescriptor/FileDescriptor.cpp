#include "FileDescriptor.hpp"

FileDescriptor::FileDescriptor(int fd) : m_Fd(fd) {}

FileDescriptor::~FileDescriptor() { close(m_Fd); }

const int& FileDescriptor::getFd() const { return m_Fd; }