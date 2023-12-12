#include "ReadFile.hpp"
#include "fcntl.h"
#include "sys/stat.h"


ReadFile::ReadFile(const std::string& filename) : FileDescriptor(0), m_FileName(filename) {
	this->m_Fd = open(filename.c_str(), O_RDONLY);

	struct stat	buf;
	if (fstat(m_Fd, &buf) < 0) {
		// TODO: exception class
		close(m_Fd);
	}
	m_FileSize = buf.st_size;

	// read에 string을 매개변수로 넘기지 못한다.
	char* tmp = new char[m_FileSize + 1];
	const ssize_t readSize = read(m_Fd, &tmp[0], m_FileSize);
	if (readSize < 0 || m_FileSize != readSize) {
		// TODO: exception class
		delete [] tmp;
		close(m_Fd);
	}
	tmp[readSize] = '\0';
	m_FileContent.assign(tmp, readSize);
	delete [] tmp;
}

ReadFile::~ReadFile() {}

const std::string&	ReadFile::getFileName() const {
	return this->m_FileName;
}

const std::string&	ReadFile::getFileContent() const {
	return this->m_FileContent;
}

const std::size_t&	ReadFile::getFileSize() const {
	return this->m_FileSize;
}