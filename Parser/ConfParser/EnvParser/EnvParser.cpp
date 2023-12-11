#include "EnvParser.hpp"
#include "../Parser/Exception/ConfParserException.hpp"


/*
	shell env   = *(env LF)
	
	env         = key "=" value

	key         = (ALPHA / "_") *(ALPHA / DIGIT / "_")

	value       = *VCHAR
*/

template <typename T>
void	ENV::envLine(ENV::envMap& dest, const std::string& env, size_t& pos) {
	std::string	key;

	if (env.empty() || (!std::isalpha(static_cast<int>(env[pos])) && env[pos] != '_')) {
		throw T(key, "Environment key is empty");
	}
	while (std::isalnum(static_cast<int>(env[pos])) || env[pos] == '_') {
		key += env[pos];
		pos++;
	}
	if (env[pos] != '=') {
		throw T(key, "Environment syntax error '=' is not found");
	}
	pos++;
	const size_t	start(pos);
	while (env[pos]) {
		pos++;
	}
	dest[key] = env.substr(start, pos - start);
	// TODO: env 값이 없는 경우도 유효한가?
	if (dest[key].empty()) {
		throw T(dest[key], "Environment value is empty");
	}
}

template <typename T>
void	ENV::EnvParser(envMap& dest, char** env) {
	size_t	idx(0);
	size_t	pos;

	while (env[idx]) {
		pos = 0;
		envLine<T>(dest, std::string(env[idx]), pos);
		idx++;
	}
}

template void	ENV::EnvParser<EnvParserException>(ENV::envMap&, char**);
template void	ENV::envLine<ConfParserException>(ENV::envMap&, const std::string&, size_t&);
