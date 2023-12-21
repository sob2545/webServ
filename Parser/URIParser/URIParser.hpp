#pragma once

#include "../PathParser/PathParser.hpp"
#include "../ABNF_utils/ABNFFunctions.hpp"
#include <string>
#include <vector>
#include <map>

namespace URI {
	

	typedef	std::map<std::string, std::vector<std::string> > QueryMap;
	// typedef	std::vector<std::string>	PathVec;

	// TODO: absPath data should be string or string vector?
	struct data {
		unsigned short	port;
		std::string     host;
		std::string		fragment;
		std::string		absPath;
		QueryMap		query;
	};
}


namespace URIParser {
	template <typename T>
	bool	hostnameParser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port);

	template <typename T>
	bool	IPv4Parser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port);

	template <typename T>
	bool	errorPageParser(const std::string& inputURI, std::size_t& pos, std::string& argument);
}

