#include "PathParser.hpp"
#include <string>

/**
 *	@brief		segment / path-segments
	*	@details	'/' 기준으로 경로를 나누어 absPath에 저장한다.
	*				segment = *pchar *( ";" param )
	*				path-segments = segment *( "/" segment )
	*				
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
*/

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

void	File_Segment(const std::string& inputURI, size_t& pos, std::string& absPath) {
	size_t	startPos(pos);

	while (pos < inputURI.size() && BNF::isPchar(inputURI, pos)) {
		pos++;
	}
	absPath += inputURI.substr(startPos, (pos - startPos));
}


void	URI_PathSegments(const std::string& inputURI, size_t& pos, std::string& absPath) {
	URI_Segment(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		absPath += "/";
		pos++;
		URI_Segment(inputURI, pos, absPath);
	}
}

void	File_PathSegments(const std::string& inputURI, size_t& pos, std::string& absPath) {
	File_Segment(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		absPath += "/";
		pos++;
		File_Segment(inputURI, pos, absPath);
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

bool	PathParser::File_AbsolutePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) != BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		absPath += "/";
		pos++;
		File_PathSegments(inputURI, pos, absPath);
		return true;
	}
}

bool	PathParser::File_RelativePath(const std::string& inputURI, size_t& pos, std::string& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) == BNF::E_RESERVED::SLASH) {
		return false;
	}
	else {
		File_PathSegments(inputURI, pos, absPath);
		return true;
	}
}