#include "File.hpp"
#include <fstream>

File::File(const std::string& fileName) {
	std::ifstream	file(fileName.c_str());

	if (!file.is_open()) {
		throw std::string("File not found");
	}
	std::getline(file, m_FileContent, '\0');
	file.close();
}

File::File(const File& copy){
	*this = copy;
}

File&	File::operator=(const File& obj) {
	m_FileContent = obj.m_FileContent;
	return *this;
}

File::~File() {
}

const std::string&	File::getFileContent() const {
	return m_FileContent;
}

void	File::writeFile(const std::string& fileName, const std::string& content, const bool& append) {
	std::ofstream	file(fileName.c_str());

	if (append) {
		file.open(fileName.c_str(), std::ios::app);
	}
	if (!file.is_open()) {
		throw std::string("File not found");
	}
	file << content;
	file.close();
}