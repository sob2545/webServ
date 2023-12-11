#include "URIParser.hpp"
#include "SchemeChecker/SchemeChecker.hpp"
#include <string>

// TODO: delete and change exception class with template
#include <iostream>
#include "../ConfParser/Parser/Exception/ConfParserException.hpp"

// TODO: compareOneCharcter throw 정의 해야됨
template <typename T>
void	compareOneCharacter(const std::string& inputURI, std::size_t& pos, const unsigned char& toCmp) {
	const std::size_t&		URILength = inputURI.length();
	const std::string	src(&inputURI[pos]);

	if (pos >= URILength || inputURI[pos] != toCmp) {
		throw ConfParserException(inputURI.substr(pos, 1), "is not uri format");
	}
	pos++;
}

/**
 *			Query setter
*/
void	query(const std::string& inputURI, std::size_t& pos, URI::QueryMap& query) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::QUESTION_MARK) {
		pos++;
	}
	else {
		return ;
	}
	std::size_t	start(pos);
	std::string	key;

	while (pos < inputURI.size() && BNF::isUric(inputURI, pos)) {
		switch (inputURI.at(pos)) {
			case (BNF::E_RESERVED::EQUALS):
				key = inputURI.substr(start, pos - start);
				start = pos + 1;
				break;
			case (BNF::E_RESERVED::AMPERSAND):
				query[key].push_back(inputURI.substr(start, pos - start));
				start = pos + 1;
				break;
			default:
				break;
		}
		pos++;
	}
	query[key].push_back(inputURI.substr(start, pos - start));
}


/**
 *		Fragment setter
*/
void	Fragment(const std::string& inputURI, std::size_t& pos, std::string& fragment) {
	if (pos < inputURI.size() && inputURI.at(pos) == '#') {
		pos++;
	} else {
		return ;
	}

	const std::size_t	start(pos);
	while (pos < inputURI.size() && BNF::isUric(inputURI, pos)) {
		pos++;
	}
	fragment = inputURI.substr(start, pos - start);
}


/**
 *			Hostname setter
*/
template <typename T>
const std::string	domainlabel(const std::string& inputURI, std::size_t& pos) {
	std::string	label;

	if (pos >= inputURI.size() || !std::isalnum(static_cast<int>(inputURI.at(pos))))
		throw ConfParserException(label, "is invalid host name");
	while (pos < inputURI.size() && (std::isalnum(static_cast<int>(inputURI.at(pos))) || inputURI.at(pos) == BNF::E_MARK::HYPHEN)) {
		label += inputURI.at(pos);
		pos++;
	}
	return label;
}

template <typename T>
void	toplabel(const std::string& host) {
	const std::size_t	dotPos = host.rfind(BNF::E_MARK::PERIOD);

	if (dotPos != std::string::npos && !std::isalpha(static_cast<int>(host.at(dotPos + 1)))) {
		throw ConfParserException(host, "is invalid host name");
	} else if (dotPos == std::string::npos && !std::isalpha(static_cast<int>(host.at(0)))) {
		throw ConfParserException(host, "is invalid host name");
	}
}

template <typename T>
bool	hostname(const std::string& inputURI, std::size_t& pos, std::string& host) {
	host += domainlabel<T>(inputURI, pos);
	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_MARK::PERIOD) {
		pos++;
		host += '.';
		host += domainlabel<T>(inputURI, pos);
	}
	toplabel<T>(host);
	return true;
}


/**
 *			Address setter
*/
bool	splitAddressNumber(const std::string& inputURI, std::size_t& pos, std::string& host) {
	if (pos >= inputURI.size() || !std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		return false;
	}
	host += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		host += inputURI.at(pos);
		pos++;
	}
	char*	endptr;
	const unsigned long addressPart = std::strtoul(host.c_str(), &endptr, 10);
	return ((addressPart <= 255) ? true : throw );
}

bool	IPv4address(const std::string& inputURI, std::size_t& pos, std::string& host) {
	for (int i = 0; i < 3;) {
		if (!splitAddressNumber(inputURI, pos, host))
			return false;
		if (pos >= inputURI.size() || inputURI.at(pos) != '.') {
			return false;
		} else if (pos < inputURI.size() && inputURI.at(pos) == '.') {
			host += '.';
			i++;
			pos++;
		}
	}
	if (!splitAddressNumber(inputURI, pos, host))
		return false;
	return true;
}

template <typename T>
bool	setHost(const std::string& inputURI, std::size_t& pos, std::string& host) {
	return (IPv4address(inputURI, pos, host) || hostname<T>(inputURI, pos, host));
}

/**
 *			Port setter
*/
template <typename T>
void	splitPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	std::string	portStr;

	if (pos >= inputURI.size() || !std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		throw T(portStr, "is invalid host name");
	}
	portStr += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		portStr += inputURI.at(pos);
		pos++;
	}
	port = static_cast<unsigned short>(std::atoi(portStr.c_str()));
}

template <typename T>
bool	setPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::COLON) {
		pos++;
		splitPort<T>(inputURI, pos, port);
		return true;
	}
	else {
		port = 80;
		return false;
	}
}


// RFC3986 부터 authroity가 나오고 absolutePath가 나옴
// hierPart에서 port를 설정하지 않음
template <typename T>
void	hierPart(const std::string& inputURI, std::size_t& pos, std::string& argument) {
	compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::SLASH);
	compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::SLASH);

	argument += "//";
	setHost<T>(inputURI, pos, argument);
	PathParser::URI_AbsolutePath(inputURI, pos, argument);
}


/**
 *			Scheme Part
*/
bool	isValidSchemeSyntax(const std::string& inputURI, std::size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return (std::isalnum(c)
			|| inputURI[pos] == BNF::E_RESERVED::PLUS
			|| inputURI[pos] == BNF::E_MARK::HYPHEN
			|| inputURI[pos] == BNF::E_MARK::PERIOD);
}

template <typename T>
bool	scheme(const std::string& inputURI, std::size_t& pos, std::string& scheme) {
	const std::size_t	URILength = inputURI.length();

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
		return false;
	}
	return true;
}


/**
 *			URI Parsing API
*/
// setServer는 기본 포트면 false, 지정된 포트가 있으면 true
template <typename T>
bool	URIParser::setServer(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port) {
	setHost<T>(inputURI, pos, argument);
	return (setPort<T>(inputURI, pos, port));
}

template <typename T>
bool	URIParser::errorPageParser(const std::string& inputURI, std::size_t& pos, std::string& argument) {
	const std::size_t	startPos = pos;
	if (!scheme<T>(inputURI, pos, argument)) {
		pos = startPos;
		argument.clear();
		return false;
	}
	compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::COLON);
	argument += ":";
	hierPart<T>(inputURI, pos, argument);
	return true;
}


template bool	URIParser::setServer<ConfParserException>(const std::string&, std::size_t&, std::string&, unsigned short&);
template bool	URIParser::errorPageParser<ConfParserException>(const std::string&, std::size_t&, std::string&);
