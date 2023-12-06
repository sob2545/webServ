#pragma once

#include "../BNF_utils/BNFFunctions.hpp"

namespace PathParser {

	bool	URI_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath);
	bool	URI_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath);
	bool	File_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath);
	bool	File_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath);
}