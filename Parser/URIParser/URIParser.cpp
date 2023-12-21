#include "URIParser.hpp"
#include "SchemeChecker/SchemeChecker.hpp"
#include <string>

#include <iostream>
#include "../ConfParser/AConfParser/Exception/ConfParserException.hpp"


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
		throw T(label, "is invalid host name");
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
		throw T(host, "is invalid host name");
	} else if (dotPos == std::string::npos && !std::isalpha(static_cast<int>(host.at(0)))) {
		throw T(host, "is invalid host name");
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
	std::string		addressPart;
	if (pos >= inputURI.size() || !std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		return false;
	}
	while (pos < inputURI.size() && std::isdigit(static_cast<int>(inputURI.at(pos)))) {
		addressPart += inputURI.at(pos);
		pos++;
	}
	char*	endptr;
	const unsigned long addressPartNumber = std::strtoul(addressPart.c_str(), &endptr, 10);
	host += addressPart;
	return ((addressPartNumber > 255 || *endptr != '\0') ? false : true);
}

bool	IPv4address(const std::string& inputURI, std::size_t& pos, std::string& host) {
	const std::size_t	startPos = pos;

	for (int i = 0; i < 3;) {
		if (!splitAddressNumber(inputURI, pos, host)) {
			pos = startPos;
			host.clear();
			return false;
		}
		if (pos >= inputURI.size() || inputURI.at(pos) != '.') {
			pos = startPos;
			host.clear();
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
bool	IPv4address(const std::string& inputURI, std::size_t& pos, std::string& host) {
	const std::size_t	startPos = pos;

	for (int i = 0; i < 3;) {
		if (!splitAddressNumber(inputURI, pos, host)) {
			pos = startPos;
			host.clear();
			return false;
		}
		if (pos >= inputURI.size() || inputURI.at(pos) != '.') {
			pos = startPos;
			host.clear();
			return false;
		} else if (pos < inputURI.size() && inputURI.at(pos) == '.') {
			host += '.';
			i++;
			pos++;
		}
	}
	if (!splitAddressNumber(inputURI, pos, host)) {
		throw T(host, "is invalid IPv4 address");
	}
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
bool	splitPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	std::string	portStr;

	while (pos < inputURI.size()
			&& !ABNF::isWSP(inputURI, pos)
			&& !ABNF::isLF(inputURI, pos)
			&& inputURI[pos] != E_ABNF::SEMICOLON
			&& inputURI[pos] != '{'
			&& inputURI[pos] != '}') {
		portStr += inputURI.at(pos);
		pos++;
	}
	char*		endPos;
	const long	portNum = strtol(portStr.c_str(), &endPos, 10);
	(*endPos != '\0' || portStr.size() > 5 || portNum < 0 || portNum > 65535) ? throw T(portStr, "is invalid port") : 0;
	if (static_cast<unsigned short>(portNum) == 0) {
		port = 80;
		return false;
	} else {
		port = static_cast<unsigned short>(portNum);
		return true;
	}
}

template <typename T>
bool	setPort(const std::string& inputURI, std::size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::COLON) {
		pos++;
		splitPort<T>(inputURI, pos, port);
		return true;
	}
	else {
		return (splitPort<T>(inputURI, pos, port));
	}
}


// RFC3986 부터 authroity가 나오고 absolutePath가 나옴
// hierPart에서 port를 설정하지 않음
template <typename T>
void	hierPart(const std::string& inputURI, std::size_t& pos, std::string& argument) {
	ABNF::compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::SLASH);
	ABNF::compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::SLASH);

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
// hostnameParser는 기본 포트면 false, 지정된 포트가 있으면 true
template <typename T>
bool	URIParser::hostnameParser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port) {
	return (setHost<T>(inputURI, pos, argument));
}

template <typename T>
bool	URIParser::IPv4Parser(const std::string& inputURI, std::size_t& pos, std::string& argument, unsigned short& port) {
	IPv4address<T>(inputURI, pos, argument);
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
	ABNF::compareOneCharacter<T>(inputURI, pos, BNF::E_RESERVED::COLON);
	argument += ":";
	hierPart<T>(inputURI, pos, argument);
	return true;
}


template bool	URIParser::hostnameParser<ConfParserException>(const std::string&, std::size_t&, std::string&, unsigned short&);
template bool	URIParser::IPv4Parser<ConfParserException>(const std::string&, std::size_t&, std::string&, unsigned short&);
template bool	URIParser::errorPageParser<ConfParserException>(const std::string&, std::size_t&, std::string&);
