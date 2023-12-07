#include "EnvParser.hpp"

/*
	shell env   = *(env LF)
	
	env         = key "=" value

	key         = ALPHA *(ALPHA / DIGIT / "_")

	value       = *VCHAR
*/


const bool	ENV::envLine(ENV::envMap& dest, const std::string& env, size_t& pos) {
	std::string	key;

	if (env.empty() || !std::isalpha(static_cast<int>(env[pos]))) {
		return false;
	}
	while (std::isalnum(static_cast<int>(env[pos])) || env[pos] == '_') {
		key += env[pos];
		pos++;
	}
	if (env[pos] != '=') {
		return false;
	}
	pos++;
	const size_t	start(pos);
	while (env[pos]) {
		key += env[pos];
		pos++;
	}
	dest[key] = env.substr(start, pos - start);
	// TODO: env 값이 없는 경우도 유효한가?
	if (dest[key].empty()) {
		return false;
	}
	return true;
}

void	ENV::EnvParser(envMap& dest, const char** env) {
	size_t	idx(0);

	while (env[idx]) {
		envLine(dest, std::string(env[idx]));
		idx++;
	}
}
