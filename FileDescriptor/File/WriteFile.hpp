#pragma once

#include "../FileDescriptor.hpp"
#include <string>

class WriteFile : public FileDescriptor {
private:
	WriteFile(const WriteFile& other);
	WriteFile& operator=(const WriteFile& other);

protected:
	const std::string	m_FileName;

public:
	WriteFile(const std::string& fileName);
	virtual ~WriteFile();

	const std::string&	getFileName() const;
	virtual void		writeContent(const std::string& contents);
};