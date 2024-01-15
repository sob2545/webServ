#pragma once

#include "../PathParser/PathParser.hpp"
#include "../ABNF_utils/ABNFFunctions.hpp"
#include <string>
#include <vector>
#include <map>

namespace URIParser {
	typedef	std::map<std::string, std::vector<std::string> >	QueryMap_t;

	template <typename T>
	bool	hostnameParser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port);

	template <typename T>
	bool	IPv4Parser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port);

	template <typename T>
	bool	errorPageParser(const std::string& inputURI, std::size_t& pos, std::string& argument);

	template <typename T>
	void	HTTPMessageParser(const std::string& inputURI, std::size_t& pos, std::string& path, unsigned short& port);

	template <typename T>
	bool	RequestOriginFormParser(const std::string& inputURI, std::size_t& pos, std::string& path, URIParser::QueryMap_t& queryVal);
}

