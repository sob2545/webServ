#pragma once

#include <string>
#include <fstream>

class File{
private:
	std::string			m_FileContent;


public:
	File(const std::string& fileName);
	File(const File& copy);
	File&	operator=(const File& obj);
	~File();

	const std::string&	getFileContent() const;	

	void	writeFile(const std::string& fileName, const std::string& content, const bool& append);
};