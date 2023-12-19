#pragma once

#include "../WriteFile.hpp"

class AccessLogFile : public WriteFile {
private:
	AccessLogFile(const AccessLogFile& other);
	AccessLogFile& operator=(const AccessLogFile& other);

public:
	AccessLogFile(const std::string& fileName);
	virtual ~AccessLogFile();

	virtual	void	writeContent(const std::string& contents);
};