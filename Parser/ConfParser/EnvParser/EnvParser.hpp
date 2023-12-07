#pragma once

#include "Exception/EnvParserException.hpp"
#include <string>
#include <map>

namespace ENV {
	typedef std::map<std::string, std::string>		envMap;

	template <typename T>
	void	EnvParser(envMap& dest, char** env);

	template <typename T>
	void	envLine(envMap& dest, const std::string& env, size_t& pos);
}