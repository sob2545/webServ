#pragma once

#include <string>
#include <map>

namespace ENV {
	typedef std::map<std::string, std::string>		envMap;

	void	Parser(envMap& dest, const char** env);
}