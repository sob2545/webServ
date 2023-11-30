#pragma once

#include "../ConfParserException/ConfParserException.hpp"
#include <cstddef>

class IConfParser {
public:
	virtual ~IConfParser() {}
	virtual void			contextLines(const std::string& file, size_t* index) = 0;
	virtual void			argumentChecker(const std::vector<std::string>& args, const unsigned char& status, const size_t* index) = 0;
	virtual void			directives(const std::string& file, size_t* index) = 0;
	virtual bool			blockContent(const std::string& file, size_t* index) = 0;
	virtual bool			context(const std::string& file, size_t* index) = 0;
	virtual std::string		argument(const std::string& file, size_t* index, const unsigned char& status) = 0;
	virtual unsigned char	directiveNameChecker(const std::string& name, size_t* index) = 0;
	virtual unsigned char	directiveName(const std::string& file, size_t* index) = 0;
};