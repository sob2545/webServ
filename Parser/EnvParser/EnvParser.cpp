#include "EnvParser.hpp"
#include <cstddef>

/*
	shell env   = *(env LF)
	
	env         = key "=" value

	key         = ALPHA *(ALPHA / DIGIT / "_")

	value       = *VCHAR
*/

void	envLine(ENV::envMap& dest, const char* env) {
	size_t	pos(0);

	key(dest, env, pos);
	if (env[pos] && env[pos] == '=') {
		value(dest, env, pos);
	} else {
		// throw;
	}
}

void	ENV::Parser(envMap& dest, const char** env) {
	size_t	idx(0);

	while (env[idx]) {
		envLine(dest, env[idx]);
		idx++;
	}
}
