#include "ReadFile.hpp"
#include "fcntl.h"
#include "sys/stat.h"
#include <cstddef>
#include <sys/_types/_ssize_t.h>

ReadFile::ReadFile(const std::string& filename) : FileDescriptor(0), m_FileName(filename) {
	this->m_Fd = open(filename.c_str(), O_RDONLY);

	struct stat	buf;
	if (fstat(m_Fd, &buf) < 0) {
		// TODO: exception class
		close(m_Fd);
	}
	m_FileSize = buf.st_size;

	const ssize_t readSize = read(m_Fd, &m_FileContent[0], m_FileSize);
	if (readSize < 0) {
		// TODO: exception class
		close(m_Fd);
	}
}

ReadFile::~ReadFile() {}

const std::string&	ReadFile::getFileName() const {
	return this->m_FileName;
}

const std::string&	ReadFile::getFileContent() const {
	return this->m_FileContent;
}

const size_t&	ReadFile::getFileSize() const {
	return this->m_FileSize;
}