#include "URIParser.hpp"
#include "SchemeChecker/SchemeChecker.hpp"
#include <string>


// TODO: URI 매개변수를 std::string으로 변경
// TODO: 다른 곳에서 사용하는 함수만 BNFdata에 정의


/**
 *  ======================== Elements Section ========================
 *  @brief Pchar(unreserved, escaped, PcharReserved) / PcharReserved / reserved / mark / unreserved / escaped(HexDigit) / uric
 *                       		                  	  | <------------ special letters ------------> |
 *  @param inputURI:	입력받은 URI
 *  @param pos:			현재 위치
 *  @return:			각각의 함수에 따라 true / false
*/

/**
 *  ======================== Util function ========================
 *  @brief				isValidSchemeSyntax / splitAddressNumber / splitPort
 *
*/

const bool	isUric(const std::string& inputURI, size_t& pos) {
	return (BNF::isPchar(inputURI, pos)
			|| inputURI.at(pos) == BNF::E_RESERVED::SLASH
			|| inputURI.at(pos) == BNF::E_RESERVED::QUESTION_MARK);
}

const bool	isValidSchemeSyntax(const std::string& inputURI, size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return (std::isalnum(c) || inputURI[pos] == '+' || inputURI[pos] == '-' || inputURI[pos] == '.');
}

const bool	splitAddressNumber(const std::string& inputURI, size_t& pos, std::string& host) {
	if (pos >= inputURI.size() || !std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		return false;
	}
	host += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		host += inputURI.at(pos);
		pos++;
	}
	return true;
}

void	splitPort(const std::string& inputURI, size_t& pos, unsigned short& port) {
	std::string	portStr;
	if (pos >= inputURI.size() || !std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid port syntax");
	}
	portStr += inputURI.at(pos);
	pos++;
	while (pos < inputURI.size() && std::isdigit(static_cast<unsigned char>(inputURI.at(pos)))) {
		portStr += inputURI.at(pos);
		pos++;
	}
	port = static_cast<unsigned short>(std::atoi(portStr.c_str()));
}

/**
 * @brief 	HTTP / HTTPS / FTP / ...
 * @details	scheme = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
 * @param inputURI:	입력받은 URI
 * @param pos:		현재 위치
 * @param scheme:	저장할 URI의 scheme
*/
void	scheme(const std::string& inputURI, size_t& pos, std::string& scheme) {
	std::string	newScheme;

	if (std::isalpha(inputURI.at(pos))) {
		newScheme += inputURI[pos++];
		while (pos < inputURI.size() && isValidSchemeSyntax(inputURI, pos)) {
			newScheme += inputURI[pos];
			pos++;
		}
	}
	if (newScheme.empty() || !SchemeChecker::instance().isValidScheme(newScheme))
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid scheme syntax");

	scheme = newScheme;
}

/**
 *	@brief		domainlabel / toplabel
	*	@details	domainlabel = alphanum / alphanum *( alphanum / "-" ) alphanum
	*				toplabel	= ALPHA / ALPHA *( alphanum / "-" ) alphanum
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
	*	@return:			각각의 함수에 따라 true / false
*/
const std::string	domainlabel(const std::string& inputURI, size_t& pos) {
	std::string	label;

	if (pos >= inputURI.size() || !std::isalnum(static_cast<unsigned char>(inputURI.at(pos))))
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid domainlabel syntax");
	while (pos < inputURI.size() && (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == BNF::E_MARK::HYPHEN)) {
		label += inputURI.at(pos);
		pos++;
	}
	return label;
}

void	toplabel(const std::string& inputURI, size_t& pos, const std::string& host) {
	const size_t	dotPos = host.rfind(BNF::E_MARK::PERIOD);

	if (dotPos != std::string::npos && !std::isalpha(static_cast<unsigned char>(host.at(dotPos + 1)))) {
		throw Utils::errorMessageGenerator(inputURI, dotPos + 1, "is invalid toplabel syntax");
	} else if (dotPos == std::string::npos && !std::isalpha(static_cast<unsigned char>(host.at(0)))) {
		throw Utils::errorMessageGenerator(inputURI, (pos - host.length()), "is invalid toplabel syntax");
	}
}

/**
 *	@brief		hostname / IPv4address
	*	@details	hostname	= *( domainlabel "." ) toplabel [ "." ]
	*				IPv4address = 1*digit "." 1*digit "." 1*digit "." 1*digit
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
	*	@return:			각각의 함수에 따라 true / false
*/
const bool	hostname(const std::string& inputURI, size_t& pos, std::string& host) {
	host += domainlabel(inputURI, pos);
	while (pos < inputURI.size() && inputURI.at(pos) == BNF::E_MARK::PERIOD) {
		pos++;
		host += '.';
		host += domainlabel(inputURI, pos);
	}
	toplabel(inputURI, pos, host);
	return true;
}

const bool	IPv4address(const std::string& inputURI, size_t& pos, std::string& host) {
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

/**
 *	@brief		server = host [port]
	*	@details	host = hostname / IPv4address
	*				port = ":" *digit
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
	*	@return:			각각의 함수에 따라 true / false

*/
const bool	host(const std::string& inputURI, size_t& pos, std::string& host) {
	return (IPv4address(inputURI, pos, host) || hostname(inputURI, pos, host));
}

void	port(const std::string& inputURI, size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::COLON) {
		pos++;
		splitPort(inputURI, pos, port);
	}
	else
		port = 80;
}

const bool	setPortDetermineDefault(const std::string& inputURI, size_t& pos, unsigned short& port) {
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

void	server(const std::string& inputURI, size_t& pos, URI::data& uri) {
	host(inputURI, pos, uri.host);
	port(inputURI, pos, uri.port);
}

const bool	URIParser::server(const std::string& inputURI, size_t& pos, std::string& hostname, unsigned short& server_port) {
	host(inputURI, pos, hostname);
	return (setPortDetermineDefault(inputURI, pos, server_port));
}

const bool	URIParser::server(const std::string& inputURI, size_t& pos, std::string& hostname) {
	return (host(inputURI, pos, hostname));
}


// TODO: '//' 처리하면 안 되는데, netPath에서 처리하고 있음
// netPath를 먼저 호출하지 않고 나중에 호출하여 absPath에서 '/'를 확인하고 netPath가 불려지도록 바꿔야 됨
const bool	netPath(const std::string& inputURI, size_t& pos, URI::data& uri) {
	/*
	Utils::compareOneCharacter(inputURI, pos, E_RESERVED::SLASH);
	if ((pos < inputURI.size()) && (inputURI.at(pos) != E_RESERVED::SLASH)) {
		--pos;
		return false;
	}
	pos++;
	*/
	server(inputURI, pos, uri);
	PathParser::URI_AbsolutePath(inputURI, pos, uri.absPath);
	return (true);
}

/**
 *	@brief		query
	*	@details	query = *( pchar / "/" / "?" )
	*				'?' 기준으로 key와 value를 나누며, '&'가 나오는 경우 여러 값을 query에 저장한다.
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
*/
void	query(const std::string& inputURI, size_t& pos, URI::data& uri) {
	if (pos < inputURI.size() && inputURI.at(pos) == BNF::E_RESERVED::QUESTION_MARK) {
		pos++;
	}
	else {
		return ;
	}
	size_t	start(pos);
	std::string	key;
	while (pos < inputURI.size() && isUric(inputURI, pos)) {
		switch (inputURI.at(pos)) {
			case (BNF::E_RESERVED::EQUALS):
				key = inputURI.substr(start, pos - start);
				start = pos + 1;
				break;
			case (BNF::E_RESERVED::AMPERSAND):
				uri.query[key].push_back(inputURI.substr(start, pos - start));
				start = pos + 1;
				break;
			default:
				break;
		}
		pos++;
	}
	uri.query[key].push_back(inputURI.substr(start, pos - start));
}

/**
 *	@brief		hier-part / absoluteURI
	*	@details	hier-part = net-path / abs-path [ "?" query ]
	*				absoluteURI = scheme ":" hier-part [ "?" query ]
*/
void	hierPart(const std::string& inputURI, size_t& pos, URI::data& uri) {
	if (!PathParser::URI_AbsolutePath(inputURI, pos, uri.absPath) && !netPath(inputURI, pos, uri)) {
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid host syntax");
	}
	query(inputURI, pos, uri);
}

void	absoluteURI(const std::string& inputURI, size_t& pos, URI::data& uri) {
	/*
		*  Scheme part is not required
	scheme(inputURI, pos, uri.scheme);
	Utils::compareOneCharacter(inputURI, pos, E_RESERVED::COLON);
	*/
	hierPart(inputURI, pos, uri);
}

void	Fragment(const std::string& inputURI, size_t& pos, URI::data& uri) {
	if (pos < inputURI.size() && inputURI.at(pos) == '#') {
		pos++;
	} else {
		return ;
	}

	const size_t	start(pos);
	while (pos < inputURI.size() && isUric(inputURI, pos)) {
		pos++;
	}
	uri.fragment = inputURI.substr(start, pos - start);
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

const bool	fullURIParser(const std::string& content, size_t& pos, std::string& argument) {

}

const bool	URIParser::isValidURI(const std::string& inputURI, std::string& hostStr) {
	size_t	pos(0);
	host(inputURI, pos, hostStr);
	return ((inputURI.size() == hostStr.size()) ? true : false);
}
