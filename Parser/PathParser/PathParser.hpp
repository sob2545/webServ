#pragma once

#include "../BNF_utils/BNFFunctions.hpp"

/*
	\ " ' * ; - ? [ ] ( ) ~ ! $ { } &lt > # @ & | space tab newline
*/

namespace E_PATH {
	enum E_PATH {
		FOWARD_SLASH = '\\',
		DOUBLE_QUOTE = '\"',
		SINGLE_QUOTE = '\'',
		ASTERISK = '*',
		SEMICOLON = ';',
		COLON = ':',
		LBRACE = '{',
		RBRACE = '}',
		LF = '\n',
		EOL = '\0'
	};
}


template <typename T>
void	File_Segment(const std::string& inputURI, size_t& pos, std::string& absPath);

template <typename T>
void	File_PathSegments(const std::string& inputURI, size_t& pos, std::string& absPath);

namespace PathParser {
	bool	URI_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath);
	bool	URI_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath);

	template <typename T>
	bool	File_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath);

	template <typename T>
	bool	File_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath);
}