#pragma once

#include "Color.hpp"
#include <sstream>
#include <string>

namespace Utils {
	bool		compareOneCharacter(const std::string& inputURI, size_t& pos, const unsigned char toCmp);
	const std::string	errorMessageGenerator(const std::string& inputURI, const int& pos, const std::string& message);
}
