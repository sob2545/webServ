#include "ErrorLogFile.hpp"

ErrorLogFile::ErrorLogFile(const std::string& fileName)
 : WriteFile(fileName)
{}

ErrorLogFile::~ErrorLogFile() {}

void	ErrorLogFile::writeContent(const std::string& contents) {
	// TODO: 시간 받아와서 ErrorLog 형식 맞춰서 contents에 대한 내용 적어주기 (매개변수도 생각해봐야 됨)
}