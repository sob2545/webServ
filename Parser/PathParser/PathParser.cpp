#include "PathParser.hpp"
#include "../ABNF_utils/ABNFFunctions.hpp"
#include "../ConfParser/Parser/Exception/ConfParserException.hpp"
#include <string>

// TODO: delete
#include <iostream>
/**
 *	@brief		segment / path-segments
	*	@details	'/' 기준으로 경로를 나누어 absPath에 저장한다.
	*				segment = *pchar *( ";" param )
	*				path-segments = segment *( "/" segment )
	*				
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
*/

bool	isValidPath(const unsigned char& c) {
	switch (c) {
		case (E_PATH::FOWARD_SLASH):
		case (E_PATH::DOUBLE_QUOTE):
		case (E_PATH::SINGLE_QUOTE):
		case (E_PATH::ASTERISK):
		case (E_PATH::SEMICOLON):
		case (E_PATH::COLON):
		case (E_PATH::LBRACE):
		case (E_PATH::RBRACE):
		case (E_PATH::LF):
		case (E_PATH::EOL):
			return false;
	}
	return true;
}

void	URI_Segment(const std::string& inputURI, size_t& pos, std::string& absPath) {
	size_t	startPos(pos);

	while (pos < inputURI.size() && BNF::isPchar(inputURI, pos)) {
		pos++;
	}
	absPath += inputURI.substr(startPos, (pos - startPos));

	// URI has parameter rule  e.g) `absPath;paramKey1=paramArg1;paramKey2=paramArg2`
	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SEMICOLON) {
		startPos = pos;
		pos++;
		while (pos < inputURI.size() && BNF::isPchar(inputURI, pos)) {
			pos++;
		}
		absPath += inputURI.substr(startPos, (pos - startPos));
	}
}

void	URI_PathSegments(const std::string& inputURI, size_t& pos, std::string& absPath) {
	URI_Segment(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		absPath += "/";
		pos++;
		URI_Segment(inputURI, pos, absPath);
	}
}


template <typename T>
void	File_Segment(const std::string& inputURI, size_t& pos, std::string& absPath) {
	while (pos < inputURI.size() && !ABNF::isWSP(inputURI, pos)
			&& inputURI[pos] != BNF::E_RESERVED::SLASH
			&& inputURI[pos] != BNF::E_RESERVED::SEMICOLON
			&& inputURI[pos] != E_ABNF::LF) {
		absPath += inputURI[pos];
		(isValidPath(inputURI[pos])) ? pos++ : throw T(absPath, "Invalid Path");
	}
}

template <typename T>
void	File_PathSegments(const std::string& inputURI, size_t& pos, std::string& absPath) {
	const size_t	startPos(pos);
	File_Segment<T>(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		absPath += "/";
		pos++;
		File_Segment<T>(inputURI, pos, absPath);
	}
}

/**
 *	@brief		abs-path / net-path
	*	@details	abs-path = "/" path-segments
	*				net-path = "//" authority [ abs-path ]
	*
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
	*	@return:			각각의 함수에 따라 true / false
*/

/*				============ absPath backup for get param with string vector ===========

void	segment(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	size_t	startPos(pos);

	while (pos < inputURI.size() && BNF::isPchar(inputURI, pos)) {
		pos++;
	}
	absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SEMICOLON) {
		startPos = pos;
		pos++;
		while (pos < inputURI.size() && BNF::isPchar(inputURI, pos)) {
			pos++;
		}
		absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	}
}

void	pathSegments(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	segment(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		pos++;
		segment(inputURI, pos, absPath);
	}
}

bool	PathParser::absPath(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) != BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		pos++;
		pathSegments(inputURI, pos, absPath);
		return true;
	}
}
*/

bool	PathParser::URI_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) != BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		absPath += "/";
		pos++;
		URI_PathSegments(inputURI, pos, absPath);
		return true;
	}
}

bool	PathParser::URI_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		URI_PathSegments(inputURI, pos, absPath);
		return true;
	}
}

template <typename T>
bool	PathParser::File_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) != BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		absPath += "/";
		pos++;
		File_PathSegments<T>(inputURI, pos, absPath);
		return true;
	}
}

template <typename T>
bool	PathParser::File_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size()
			|| inputURI.at(pos) == BNF::E_RESERVED::SLASH
			|| inputURI.at(pos) == BNF::E_RESERVED::SEMICOLON) {
		return false;
	}
	else {
		File_PathSegments<T>(inputURI, pos, absPath);
		return true;
	}
}

template bool PathParser::File_AbsolutePath<ConfParserException>(const std::string&, size_t&, std::string&);
template bool PathParser::File_RelativePath<ConfParserException>(const std::string&, size_t&, std::string&);