#include "URIParser.hpp"
#include "SchemeChecker/SchemeChecker.hpp"
#include <cctype>
#include <cstddef>
#include <string>


// TODO: URI 매개변수를 std::string으로 변경
// TODO: 다른 곳에서 사용하는 함수만 BNFdata에 정의

// TODO: compareOneCharcter throw 정의 해야됨
void	compareOneCharacter(const std::string& inputURI, size_t& pos, const unsigned char& toCmp) {
	const size_t&	URILength = inputURI.length();

	if (pos >= URILength || inputURI[pos] != toCmp) {
		throw ;
	}
	pos++;
}



const bool	setHost(const std::string& inputURI, size_t& pos, std::string& host) {
	return (IPv4address(inputURI, pos, host) || hostname(inputURI, pos, host));
}

/**
 *			Port setter
*/
void	setPort(const std::string& inputURI, size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::COLON) {
		pos++;
		splitPort(inputURI, pos, port);
	}
	else
		port = 80;
}

const bool	setPortCheck(const std::string& inputURI, size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::COLON) {
		pos++;
		splitPort(inputURI, pos, port);
		return true;
	}
	else {
		port = 80;
		return false;
	}
}


void	server(const std::string& inputURI, size_t& pos, std::string& argument) {
	setHost(inputURI, pos, argument);
	setPort(inputURI, pos, argument);
}

void	hierPart(const std::string& inputURI, size_t& pos, std::string& argument) {
	compareOneCharacter(inputURI, pos, BNF::E_RESERVED::SLASH);
	compareOneCharacter(inputURI, pos, BNF::E_RESERVED::SLASH);

	server(inputURI, pos, argument);
}


/**
 *			Scheme Part
*/
const bool	isValidSchemeSyntax(const std::string& inputURI, size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return (std::isalnum(c)
			|| inputURI[pos] == BNF::E_RESERVED::PLUS
			|| inputURI[pos] == BNF::E_MARK::HYPHEN
			|| inputURI[pos] == BNF::E_MARK::PERIOD);
}

const bool	scheme(const std::string& inputURI, size_t& pos, std::string& scheme) {
	const size_t	URILength = inputURI.length();

	if (std::isalpha(inputURI.at(pos))) {
		scheme += inputURI[pos++];
		while (pos < URILength && isValidSchemeSyntax(inputURI, pos)) {
			scheme += inputURI[pos];
			pos++;
		}
	} else {
		return false;
	}

	if 	(scheme.empty() || !SchemeChecker::instance().isValidScheme(scheme)) {
		throw ;
	}
	return true;
}


/**
 *			URI Parsing API
*/
const bool	fullURIParser(const std::string& inputURI, size_t& pos, std::string& argument) {
	if (!scheme(inputURI, pos, argument)) {
		return false;
	}
	compareOneCharacter(inputURI, pos, BNF::E_RESERVED::COLON);
	hierPart();


	return true;
}

const bool	URIParser::isValidHost(const std::string& inputURI, std::string& hostStr) {
	size_t	pos(0);

	host(inputURI, pos, hostStr);
	return (inputURI.size() == hostStr.size());
}

URI::data	URIParser::Parser(const std::string& inputURI) {
	URI::data	uri;
	size_t		pos(0);

	absoluteURI(inputURI, pos, uri);
	// TODO: Fragment 보기전에 빈 `//` 있는지 체크할 것
	// if (uri.absPath.size() && uri.absPath[uri.absPath.size() - 1].empty()) {
	// 	uri.absPath.pop_back();
	// }
	Fragment(inputURI, pos, uri);

	return uri;
}
