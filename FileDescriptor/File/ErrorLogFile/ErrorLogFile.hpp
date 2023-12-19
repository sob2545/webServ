#pragma once

#include "../WriteFile.hpp"

class ErrorLogFile : public WriteFile {
private:
	ErrorLogFile(const ErrorLogFile& other);
	ErrorLogFile& operator=(const ErrorLogFile& other);

public:
	ErrorLogFile(const std::string& fileName);
	virtual ~ErrorLogFile();

	virtual	void	writeContent(const std::string& contents);
};