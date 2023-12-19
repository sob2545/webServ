#include "WriteFile.hpp"
#include <sys/fcntl.h>

// TODO: Implement throw

WriteFile::WriteFile(const std::string& filename)
: FileDescriptor(0), m_FileName(filename)
{
	this->m_Fd = open(m_FileName.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);

	(m_Fd < 0) ? throw : 0;
}

WriteFile::~WriteFile() {}

const std::string&	WriteFile::getFileName() const {
	return this->m_FileName;
}

void	WriteFile::writeContent(const std::string& contents) {
	const std::size_t writeSize = write(m_Fd, contents.c_str(), contents.length());
	(writeSize < 0) ? throw : 0;
}
