#pragma once

#include "../../utils/utilFunctions.hpp"
#include "../PathParser/PathParser.hpp"
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
	bool	setServer(const std::string& inputURI, size_t& pos, std::string& argument, unsigned short& port);
	bool	errorPageParser(const std::string& inputURI, size_t& pos, std::string& argument);
}

