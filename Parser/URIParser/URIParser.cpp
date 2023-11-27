#include "URIParser.hpp"
#include "BNF_utils/SchemeChecker.hpp"


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
bool	isPcharReserved(const std::string& inputURI, size_t& pos) {
	switch (inputURI.at(pos)) {
		case (URI::Reserved::COLON):
		case (URI::Reserved::AT_SIGN):
		case (URI::Reserved::AMPERSAND):
		case (URI::Reserved::EQUALS):
		case (URI::Reserved::PLUS):
		case (URI::Reserved::DOLLAR_SIGN):
		case (URI::Reserved::COMMA):
			return true;
		default :
			return false;
	}
}

bool	isReserved(const std::string& inputURI, size_t& pos) {
	switch (inputURI.at(pos)) {
		case (URI::Reserved::SEMICOLON):
		case (URI::Reserved::SLASH):
		case (URI::Reserved::QUESTION_MARK):
		case (URI::Reserved::COLON):
		case (URI::Reserved::AT_SIGN):
		case (URI::Reserved::AMPERSAND):
		case (URI::Reserved::EQUALS):
		case (URI::Reserved::PLUS):
		case (URI::Reserved::DOLLAR_SIGN):
		case (URI::Reserved::COMMA):
			return true;
		default :
			return false;
	}
}

bool	isMark(const std::string& inputURI, size_t& pos) {
	switch (inputURI.at(pos)) {
		case (URI::Mark::HYPHEN):
		case (URI::Mark::UNDERSCORE):
		case (URI::Mark::PERIOD):
		case (URI::Mark::EXCLAMATION_MARK):
		case (URI::Mark::TILDE):
		case (URI::Mark::ASTERISK):
		case (URI::Mark::SINGLE_QUOTE):
		case (URI::Mark::LEFT_PARENTHESIS):
		case (URI::Mark::RIGHT_PARENTHESIS):
			return true;
		default:
			return false;
	}
}

bool	isUnreserved(const std::string& inputURI, size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return ((std::isalnum(c) || isMark(inputURI, pos)) ? true : false);
}

bool	isEscaped(const std::string& inputURI, size_t& pos) {
	if (inputURI.at(pos) == '%') {
		if (pos + 2 < inputURI.size()) {
			return (isxdigit(inputURI[pos + 1]) && isxdigit(inputURI[pos + 2])) ? true : false;
		}
		pos += 2;
	}
	return false;
}

bool	isPchar(const std::string& inputURI, size_t& pos) {
	if (isUnreserved(inputURI, pos) || isEscaped(inputURI, pos) || isPcharReserved(inputURI, pos)) {
		return true;
	} else {
		return false;
	}
}

bool	isUric(const std::string& inputURI, size_t& pos, URI::data& uri) {
	return (isPchar(inputURI, pos)
			|| inputURI.at(pos) == URI::Reserved::SLASH
			|| inputURI.at(pos) == URI::Reserved::QUESTION_MARK);
}

/**
 *  ======================== Util function ========================
 *  @brief				isValidSchemeSyntax / splitAddressNumber / splitPort
 *
*/


bool	isValidSchemeSyntax(const std::string& inputURI, size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return (std::isalnum(c) || inputURI[pos] == '+' || inputURI[pos] == '-' || inputURI[pos] == '.');
}

bool	splitAddressNumber(const std::string& inputURI, size_t& pos, std::string& host) {
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
 *  ======================== BNF Section ========================

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
std::string	domainlabel(const std::string& inputURI, size_t& pos) {
	std::string	label;

	if (pos >= inputURI.size() || !std::isalnum(static_cast<unsigned char>(inputURI.at(pos))))
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid domainlabel syntax");
	while (pos < inputURI.size() && (std::isalnum(static_cast<unsigned char>(inputURI.at(pos))) || inputURI.at(pos) == URI::Mark::HYPHEN)) {
		label += inputURI.at(pos);
		pos++;
	}
	return label;
}

void	toplabel(const std::string& inputURI, size_t& pos, const std::string& host) {
	const size_t	dotPos = host.rfind(URI::Mark::PERIOD);

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
bool	hostname(const std::string& inputURI, size_t& pos, std::string& host) {
	host += domainlabel(inputURI, pos);
	while (pos < inputURI.size() && inputURI.at(pos) == URI::Mark::PERIOD) {
		pos++;
		host += '.';
		host += domainlabel(inputURI, pos);
	}
	toplabel(inputURI, pos, host);
	return true;
}

bool	IPv4address(const std::string& inputURI, size_t& pos, std::string& host) {
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
void	host(const std::string& inputURI, size_t& pos, std::string& host) {
	IPv4address(inputURI, pos, host) || hostname(inputURI, pos, host);
}

void	port(const std::string& inputURI, size_t& pos, unsigned short& port) {
	if (pos < inputURI.size() && inputURI.at(pos) == URI::Reserved::COLON) {
		pos++;
		splitPort(inputURI, pos, port);
	}
	else
		port = 80;
}

void	server(const std::string& inputURI, size_t& pos, URI::data& uri) {
	host(inputURI, pos, uri.host);
	port(inputURI, pos, uri.port);
}

/**
 *	@brief		segment / path-segments
	*	@details	'/' 기준으로 경로를 나누어 absPath에 저장한다.
	*				segment = *pchar *( ";" param )
	*				path-segments = segment *( "/" segment )
	*				
	*	@param inputURI:	입력받은 URI
	*	@param pos:			현재 위치
*/
void	segment(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	size_t	startPos(pos);

	while (pos < inputURI.size() && isPchar(inputURI, pos)) {
		pos++;
	}
	absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	while (pos < inputURI.size() && inputURI.at(pos) == URI::Reserved::SEMICOLON) {
		startPos = pos;
		pos++;
		while (pos < inputURI.size() && isPchar(inputURI, pos)) {
			pos++;
		}
		absPath.push_back(inputURI.substr(startPos, (pos - startPos)));
	}
}

void	pathSegments(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	segment(inputURI, pos, absPath);

	while (pos < inputURI.size() && inputURI.at(pos) == URI::Reserved::SLASH) {
		pos++;
		segment(inputURI, pos, absPath);
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
bool	absPath(const std::string& inputURI, size_t& pos, std::vector<std::string>& absPath) {
	if (pos >= inputURI.size() || inputURI.at(pos) != URI::Reserved::SLASH) {
		return false;
	}
	else {
		pos++;
		pathSegments(inputURI, pos, absPath);
		return true;
	}
}

// TODO: '//' 처리하면 안 되는데, netPath에서 처리하고 있음
// netPath를 먼저 호출하지 않고 나중에 호출하여 absPath에서 '/'를 확인하고 netPath가 불려지도록 바꿔야 됨
bool	netPath(const std::string& inputURI, size_t& pos, URI::data& uri) {
	/*
	Utils::compareOneCharacter(inputURI, pos, Reserved::SLASH);
	if ((pos < inputURI.size()) && (inputURI.at(pos) != Reserved::SLASH)) {
		--pos;
		return false;
	}
	pos++;
	*/
	server(inputURI, pos, uri);
	absPath(inputURI, pos, uri.absPath);
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
	if (pos < inputURI.size() && inputURI.at(pos) == URI::Reserved::QUESTION_MARK) {
		pos++;
	}
	else {
		return ;
	}
	size_t	start(pos);
	std::string	key;
	while (pos < inputURI.size() && isUric(inputURI, pos, uri)) {
		switch (inputURI.at(pos)) {
			case (URI::Reserved::EQUALS):
				key = inputURI.substr(start, pos - start);
				start = pos + 1;
				break;
			case (URI::Reserved::AMPERSAND):
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
	if (!absPath(inputURI, pos, uri.absPath) && !netPath(inputURI, pos, uri)) {
		throw Utils::errorMessageGenerator(inputURI, pos, "is invalid host syntax");
	}
	query(inputURI, pos, uri);
}

void	absoluteURI(const std::string& inputURI, size_t& pos, URI::data& uri) {
	/*
		*  Scheme part is not required
	scheme(inputURI, pos, uri.scheme);
	Utils::compareOneCharacter(inputURI, pos, Reserved::COLON);
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
	while (pos < inputURI.size() && isUric(inputURI, pos, uri)) {
		pos++;
	}
	uri.fragment = inputURI.substr(start, pos - start);
}

URI::data	URIParser::Parser(const std::string& inputURI) {
	URI::data	uri;
	size_t		pos(0);

	absoluteURI(inputURI, pos, uri);
	if (uri.absPath.size() && uri.absPath[uri.absPath.size() - 1].empty()) {
		uri.absPath.pop_back();
	}
	Fragment(inputURI, pos, uri);

	return uri;
}

bool	isValidURI(const std::string& inputURI, std::string& hostStr) {
	size_t	pos(0);
	host(inputURI, pos, hostStr);
	return ((inputURI.size() == hostStr.size()) ? true : false);
}
