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
	const bool	isValidHost(const std::string& inputURI, std::string& host);
	const bool	server(const std::string& inputURI, size_t& pos, std::string& host, unsigned short& port);
	const bool	server(const std::string& inputURI, size_t& pos, std::string& hostname);
	URI::data	Parser(const std::string& inputURI);
}

