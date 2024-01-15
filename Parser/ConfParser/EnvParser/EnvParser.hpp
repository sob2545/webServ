#pragma once

#include "Exception/EnvParserException.hpp"
#include <string>
#include <map>

namespace ENV {
	typedef std::map<std::string, std::string>		EnvMap_t;

	template <typename T>
	void	EnvParser(EnvMap_t& dest, char** env);

	template <typename T>
	void	envLine(EnvMap_t& dest, const std::string& env, std::size_t& pos);
}