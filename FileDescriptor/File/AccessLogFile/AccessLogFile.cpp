#include "AccessLogFile.hpp"

AccessLogFile::AccessLogFile(const std::string& fileName)
 : WriteFile(fileName)
{}

AccessLogFile::~AccessLogFile() {}

void	AccessLogFile::writeContent(const std::string& contents) {
	// TODO: 시간 받아와서 AccessLog 형식 맞춰서 contents에 대한 내용 적어주기 (매개변수도 생각해봐야 됨)
}