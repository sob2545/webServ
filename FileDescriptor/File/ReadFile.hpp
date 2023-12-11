#pragma once

#include "../FileDescriptor.hpp"
#include <cstddef>
#include <string>

class ReadFile : public FileDescriptor {
protected:
	const std::string	m_FileName;
	std::size_t				m_FileSize;
	std::string			m_FileContent;

public:
	const std::string&	getFileName() const;
	const std::size_t&		getFileSize() const;
	const std::string&	getFileContent() const;

	ReadFile(const std::string& fileName);
	~ReadFile();
};