#pragma once

#include <sstream>
#include <string>

namespace Utils {
	void		compareOneCharacter(const std::string& inputURI, size_t& pos, const unsigned char toCmp);
	std::string	errorMessageGenerator(const std::string& inputURI, const int pos, const std::string& message);
}
