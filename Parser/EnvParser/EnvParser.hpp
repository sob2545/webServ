#pragma once

#include <string>
#include <map>

namespace ENV {
	typedef std::map<std::string, std::string>		envMap;

	void		EnvParser(envMap& dest, const char** env);
	const bool	envLine(envMap& dest, const std::string& env);
}