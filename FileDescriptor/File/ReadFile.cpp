#include "ReadFile.hpp"
#include "fcntl.h"
#include "sys/stat.h"
#include "../../Utils/SmartPointer.hpp"


ReadFile::ReadFile(const std::string& filename) : FileDescriptor(0), m_FileName(filename) {
	this->m_Fd = open(filename.c_str(), O_RDONLY);

	// TODO: Implement throw
	(m_Fd < 0) ? throw FileException("file open fail") : 0;
	struct stat	buf;
	if (fstat(m_Fd, &buf) < 0) {
		throw FileException("error in file status");
		close(m_Fd);
	}
	m_FileSize = buf.st_size;

	// read에 string을 매개변수로 넘기지 못한다.
	ft::unique_ptr<char[]>	tmp(::new char[m_FileSize + 1]);
	const ssize_t readSize = read(m_Fd, &tmp[0], m_FileSize);
	if (readSize < 0 || m_FileSize != readSize) {
		throw FileException("file read fail");
		close(m_Fd);
	}
	tmp[readSize] = '\0';
	m_FileContent.assign(tmp.get(), readSize);
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