#pragma once

#include "../ConfParserException/ConfParserException.hpp"

class IConfParser {
private:
public:
	virtual ~IConfParser() {}
	virtual void				argumentChecker(const std::vector<std::string>& args, const unsigned char& status, const size_t* index) = 0;
	virtual void				directives(const std::string& file, size_t* index) = 0;
	virtual const bool			contextLines(const std::string& file, size_t* index) = 0;
	virtual const bool			context(const std::string& file, size_t* index) = 0;
	virtual const std::string	argument(const std::string& file, size_t* index, const unsigned char& status) = 0;
	virtual const unsigned char	directiveNameChecker(const std::string& name, size_t* index) = 0;
	virtual const unsigned char	directiveName(const std::string& file, size_t* index) = 0;
};