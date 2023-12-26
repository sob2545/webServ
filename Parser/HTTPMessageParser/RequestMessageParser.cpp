#include "RequestMessageParser.hpp"

HTTP::RequestMessageParser::MethodMap	HTTP::RequestMessageParser::m_MethodMap;
HTTP::RequestMessageParser::HeaderMap	HTTP::RequestMessageParser::m_HeaderMap;

HTTP::RequestMessageParser::RequestMessageParser() {
	initMethodMap();
	initHeaderMap();
}

HTTP::RequestMessageParser::~RequestMessageParser() {}

void	HTTP::RequestMessageParser::initMethodMap() {
	m_MethodMap["GET"] = 0b00000001;
	m_MethodMap["POST"] = 0b00000010;
	m_MethodMap["DELETE"] = 0b00000100;
	m_MethodMap["PUT"] = 0b00001000;
	m_MethodMap["PATCH"] = 0b00010000;
	m_MethodMap["OPTIONS"] = 0b00100000;
}

void	HTTP::RequestMessageParser::initHeaderMap() {
	m_HeaderMap["Connection"] = 0b00000001;
	m_HeaderMap["Date"] = 0b00000010;
	m_HeaderMap["Transfer-Encoding"] = 0b00000100;
	m_HeaderMap["Host"] = 0b00001000;
	m_HeaderMap["Content-Lengths"] = 0b00010000;
	m_HeaderMap["Content-Type"] = 0b00100000;
	m_HeaderMap["Cookie"] = 0b01000000; 
}

bool	HTTP::RequestMessageParser::isDuplicatable(const unsigned short& status) {
	switch (status) {
		case (DATE):
		case (TRANSFER_ENCODING):
			return true;
	}
	return false;
}


/**
 *		Parse Utils
*/
void	HTTP::RequestMessageParser::skipWSP(const std::string& message, std::size_t& pos) {
	const std::size_t&	messageSize(message.size());

	while (pos < messageSize && ABNF::isWSP(message, pos)) {
		pos++;
	}
}

void	HTTP::RequestMessageParser::token(const std::string& message, std::size_t& pos, std::string& argument) {
	const std::size_t&	messageSize(message.size());

	while (pos < messageSize && ABNF::isTCHAR(message, pos)) {
		argument += message[pos];
	}
}


// TODO: Accept, Transfer-Encoding과 같은 헤더는 ','가 구분자이다.
// Cookie는 ';'가 구분자이다.
// 1. 구분자를 파싱(string으로 만드는 단계)에서 구분 후 argumentChecker에서는 유효성 검사만 할지 (status code를 넘겨서 확인)
// 2. 파싱에서는 문법에 상관없이 일단 OWS(White Space)로 구분만 한 다음 argumentChecker에서 구분 후 처리할지
//
// 1번의 경우 `arg1, arg2 arg3, ` 이런 요류를 잡기 힘듬 (추가적인 변수로 유효성에 대한 내용 저장 필요)
// 2번의 경우 순회하는 과정이 많아짐

/**
 *		Field Line (Header Part)
 *		Header의 내용들을 정의해둬서 argument, argumentChekcer와 같이 운용
*/
const std::string	HTTP::RequestMessageParser::fieldContent(const std::string& message, std::size_t& pos) {
	const std::size_t&	msgSize = message.size();
	std::string		arg;

	while (pos < msgSize && std::isprint(static_cast<int>(message[pos]))) {
		arg += std::tolower(static_cast<int>(message[pos]));
	}
	return arg;
}

void	HTTP::RequestMessageParser::fieldValue(const std::string& message, std::size_t& pos, std::vector<std::string>& args) {
	while (!ABNF::isCRLF(message, pos)) {
		args.push_back(fieldContent(message, pos));
	}
}

unsigned short	HTTP::RequestMessageParser::fieldName(const std::string& message, std::size_t& pos) {
	std::string		headerKey;

	token(message, pos, headerKey);
	HTTP::RequestMessageParser::HeaderMap::const_iterator	it = m_HeaderMap.find(headerKey);
	if (it != m_HeaderMap.end()) {
		return (0);
	}
	return it->second;
}

void	HTTP::RequestMessageParser::fieldLine(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos, bool& checkBit) {
	const std::size_t	messageSize(message.size());
	if (ABNF::isCRLF(message, pos)) {
		return (checkBit) ? static_cast<void>(0) : throw BadRequestException(); 
	}
	checkBit |= true;

	const unsigned short	fieldStatus(fieldName(message, pos));
	ABNF::compareOneCharacter(message, pos, BNF::E_RESERVED::COLON) ? 0 : throw BadRequestException();

	std::vector<std::string>	args;
	skipWSP(message, pos);
	fieldValue(message, pos, args);
	skipWSP(message, pos);
}



/**
 *		HTTP Parse
*/
void	HTTP::RequestMessageParser::HTTPVersion(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	const std::size_t	messageSize(message.size());
	std::string			HTTPName;

	for (std::size_t i(0); i < 4 && pos < messageSize; ++i, ++pos) {
		HTTPName += message[pos];
	}
	(HTTPName == "HTTP") ? 0 : throw BadRequestException();

	ABNF::compareOneCharacter(message, pos, BNF::E_RESERVED::SLASH);

	float HTTPVersion = (pos < messageSize && std::isdigit(static_cast<int>(message[pos]))) ? static_cast<float>(message[pos] - '0') : throw BadRequestException();
	ABNF::compareOneCharacter(message, ++pos, BNF::E_MARK::PERIOD) ? 0 : throw BadRequestException();
	HTTPVersion += (++pos < messageSize && std::isdigit(static_cast<int>(message[pos]))) ? (static_cast<float>(message[pos] - '0') / 10) : throw BadRequestException();
	++pos;
	(HTTPVersion > 1.1 && HTTPVersion < 0.9) ? 0 : throw HTTPVersionNotSupportException();

	if (HTTPVersion == 0.9f) {
		recipe.m_Version = 0b1;
	} else if (HTTPVersion == 1.0f) {
		recipe.m_Version = 0b10;
	} else {
		recipe.m_Version = 0b100;
	}
}


/**
 *		Method (GET / POST / DELETE ...)
 *		std::map에 key로 method를 갖고, value로 status(char)를 저장하여 비트마스킹
 *		GET / DELETE 이면 request-target을 저장할 필요 없이 바로 꺼내서 주면 됨
 *		POST의 경우 BODY의 내용까지 다 읽고 request-target에 실행해야 하므로 저장해야됨
*/
void	HTTP::RequestMessageParser::method(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	std::string	method;

	token(message, pos, method);
	HTTP::RequestMessageParser::MethodMap::const_iterator	it = m_MethodMap.find(method);
	(it != m_MethodMap.end()) ? recipe.m_Method |= it->second : throw NotImplementedException();
}


/**
 *		Start Line
*/
bool	HTTP::RequestMessageParser::OriginForm(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	try {
		URIParser::RequestOriginFormParser<HTTPRequestParsingException>(message, pos, recipe.m_RequestData.m_Path, recipe.m_RequestData.m_Query);
	} catch (HTTPRequestParsingException& e) {
		// TODO: 현재는 OriginForm만 있기 때문에 실패 시 BadRequest 에러
		throw BadRequestException();
		// return false;
	}
	return true;
}

void	HTTP::RequestMessageParser::requestTarget(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	OriginForm(recipe, message, pos);
}

void	HTTP::RequestMessageParser::requestLine(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	method(recipe, message, pos);
	ABNF::compareOneCharacter(message, pos, E_ABNF::SP) ? 0 : throw BadRequestException();
	requestTarget(recipe, message, pos);
	ABNF::compareOneCharacter(message, pos, E_ABNF::SP) ? 0 : throw BadRequestException();
	HTTPVersion(recipe, message, pos);
}



void	HTTP::RequestMessageParser::messageBody(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	// TODO: implement
}

/**
 *		Main Parser
*/
HTTP::ResponseRecipe	HTTP::RequestMessageParser::Parser(const std::string &message) {
	HTTP::ResponseRecipe	recipe;
	std::size_t				pos(0);
	bool					checkBit(false);

	try {
		requestLine(recipe, message, pos);
		while (pos < message.length() && ABNF::isCRLF(message, pos)) {
			pos += 2;
			fieldLine(recipe, message, pos, checkBit);
		}
		messageBody(recipe, message, pos);
	} catch (HTTP::BadRequestException) {
		recipe.m_Status = static_cast<unsigned char>(400);
	} catch (HTTP::MethodNotFoundException) {
		recipe.m_Status = static_cast<unsigned char>(405);
	} catch (HTTP::LengthRequriedException) {
		recipe.m_Status = static_cast<unsigned char>(411);
	} catch (HTTP::RequestEntityTooLongException) {
		recipe.m_Status = static_cast<unsigned char>(413);
	} catch (HTTP::RequestURITooLongException) {
		recipe.m_Status = static_cast<unsigned char>(414);
	} catch (HTTP::NotImplementedException) {
		recipe.m_Status = static_cast<unsigned char>(501);
	} catch (HTTP::HTTPVersionNotSupportException) {
		recipe.m_Status = static_cast<unsigned char>(505);
	}

	return (recipe);
}