#include "RequestMessageParser.hpp"
#include "HTTPException/BadRequestException.hpp"
#include "HTTPException/HTTPRequestParsingException.hpp"
#include "../../MainLoop/Client/ClientData/ResponseRecipe.hpp"
#include "../../MainLoop/MasterProcess.hpp"
#include <cstddef>
#include <string>

HTTP::RequestMessageParser::MethodMap_t	HTTP::RequestMessageParser::m_MethodMap;
HTTP::RequestMessageParser::HeaderMap_t	HTTP::RequestMessageParser::m_HeaderMap;

HTTP::RequestMessageParser::RequestMessageParser() {
	initMethodMap();
	initHeaderMap();
}

HTTP::RequestMessageParser::~RequestMessageParser() {}

void	HTTP::RequestMessageParser::initMethodMap() {
	m_MethodMap["GET"] = E_HTTP::E_METHOD::GET;
	m_MethodMap["POST"] = E_HTTP::E_METHOD::POST;
	m_MethodMap["DELETE"] = E_HTTP::E_METHOD::DELETE;
	m_MethodMap["PUT"] = E_HTTP::E_METHOD::PUT;
	m_MethodMap["PATCH"] = E_HTTP::E_METHOD::PATCH;
	m_MethodMap["OPTIONS"] = E_HTTP::E_METHOD::OPTIONS;
}

void	HTTP::RequestMessageParser::initHeaderMap() {
	m_HeaderMap["Connection"] = E_HTTP::E_HEADER::CONNECTION;
	m_HeaderMap["Date"] = E_HTTP::E_HEADER::DATE;
	m_HeaderMap["Transfer-Encoding"] = E_HTTP::E_HEADER::TRANSFER_ENCODING;
	m_HeaderMap["Host"] = E_HTTP::E_HEADER::HOST;
	m_HeaderMap["Content-Lengths"] = E_HTTP::E_HEADER::CONTENT_LENGTHS;
	m_HeaderMap["Content-Type"] = E_HTTP::E_HEADER::CONTENT_TYPE;
	m_HeaderMap["Cookie"] = E_HTTP::E_HEADER::COOKIE;
}

bool	HTTP::RequestMessageParser::isDuplicatable(const unsigned short& status) {
	switch (status) {
		case (E_HTTP::E_HEADER::CONNECTION):
		case (E_HTTP::E_HEADER::DATE):
		case (E_HTTP::E_HEADER::TRANSFER_ENCODING):
		case (E_HTTP::E_HEADER::COOKIE):
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

bool	HTTP::RequestMessageParser::argumentChecker(HTTP::ResponseRecipe& recipe, const std::vector<std::string>& args, const unsigned short& status) {
	(args.empty()) ? throw BadRequestException() : 0;

	switch (status) {
		case (E_HTTP::E_HEADER::CONNECTION): {
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
				std::map<unsigned short, void*>::iterator connectionStatus = recipe.m_HeaderMap.find((status));

				if (*it == "close") {
					if (connectionStatus != recipe.m_HeaderMap.end() && connectionStatus->second) {
						continue;
					} else if (connectionStatus != recipe.m_HeaderMap.end() && !connectionStatus->second) {
						*reinterpret_cast<bool*>(connectionStatus->second) = true;
					} else {
						recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new bool(true))));
					}
				} else if (*it == "keep-alive") {
					if (connectionStatus != recipe.m_HeaderMap.end()) {
						continue;
					}
					recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new bool(false))));
				} else {
					throw BadRequestException();
				}
			}
		}
		case (E_HTTP::E_HEADER::DATE): {
			// DateParser();
		}
		case (E_HTTP::E_HEADER::TRANSFER_ENCODING): {
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
				std::map<unsigned short, void*>::iterator encodingStatus = recipe.m_HeaderMap.find((status));

				if (encodingStatus != recipe.m_HeaderMap.end()) {
					(!encodingStatus->second) ? (*reinterpret_cast<bool*>(encodingStatus->second) = true) : 0;
				} else {
					recipe.m_HeaderMap.insert(std::make_pair(status, (static_cast<void*>(::new bool((*it == "chunked") ? true : false)))));
				}
			}
		}
		case (E_HTTP::E_HEADER::HOST): {
			// argument길이가 최대 몇 자 넘어가면 exception
			recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new std::string(args[0]))));
		}
		case (E_HTTP::E_HEADER::CONTENT_LENGTHS): {
			(args.size() != 1) ? throw BadRequestException() : 0;
			char*	endptr;
			const long		length = strtol(args[0].c_str(), &endptr, 10);
			(length <= 0) ? throw BadRequestException() : 0;
			recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new int(length))));
		}
		case (E_HTTP::E_HEADER::CONTENT_TYPE): {
			(args.size() != 1) ? throw BadRequestException() : 0;
			MasterProcess::getMIMETypes().find(args[0]) != MasterProcess::getMIMETypes().end() ?
				recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new std::string(args[0])))) : throw BadRequestException();
		}
		case (E_HTTP::E_HEADER::COOKIE): {
			if (recipe.m_HeaderMap.find(status) == recipe.m_HeaderMap.end()) {
				recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new std::map<std::string, std::string>)));
			}
			std::map<std::string, std::string>*	CookieMap_t = static_cast<std::map<std::string, std::string>* >(recipe.m_HeaderMap.find(status)->second);
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
				const std::size_t	pos = it->find("=");
				pos == std::string::npos ? throw BadRequestException() : 0;
				const std::string	key = it->substr(0, pos - 1);
				const std::string	val = it->substr(pos + 1, it->length());
				CookieMap_t->insert(std::make_pair(key, val));
			}
		}
	}
	// 정의된 헤더가 아니면 그냥 skip한다
	return (false);
}


/**
 *		Field Line (Header Part)
*/
const std::string	HTTP::RequestMessageParser::fieldContent(const std::string& message, std::size_t& pos, const unsigned short& status) {
	const std::size_t&	msgSize(message.size());
	std::string		arg;
	const char		delimiter = (status & E_HTTP::E_HEADER::TRANSFER_ENCODING || status & E_HTTP::E_HEADER::CONNECTION) ?
								E_HTTP::E_DELIMETER::COMMA : ((status & E_HTTP::E_HEADER::COOKIE) ?
																E_HTTP::E_DELIMETER::SEMICOLON : E_HTTP::E_DELIMETER::DEFAULT);

	while (pos < msgSize && ABNF::isWSP(message, pos)) {
		pos++;
	}
	while (pos < msgSize && std::isprint(static_cast<int>(message[pos]))) {
		if (message[pos] == delimiter) {
			pos += 2;
			break ;
		}
		arg += std::tolower(static_cast<int>(message[pos]));
		pos++;
	}
	return arg;
}

void	HTTP::RequestMessageParser::fieldValue(const std::string& message, std::size_t& pos, std::vector<std::string>& args, const unsigned short& status) {
	if (status & E_HTTP::E_HEADER::HOST) {
		std::string		arg;
		unsigned short	port;
		try {
			URIParser::HTTPMessageParser<HTTPRequestParsingException>(message, pos, arg, port);
		} catch (HTTPRequestParsingException& e) {
			throw BadRequestException();
		}
		static_cast<void>(port);
		args.push_back(arg);
		return ;
	}
	while (!ABNF::isCRLF(message, pos)) {
		const std::string	arg = fieldContent(message, pos, status);
		if (!arg.empty()) {
			args.push_back(arg);
		}
	}
}

unsigned short	HTTP::RequestMessageParser::fieldName(const std::string& message, std::size_t& pos) {
	std::string		headerKey;

	token(message, pos, headerKey);
	HTTP::RequestMessageParser::HeaderMap_t::const_iterator	it = m_HeaderMap.find(headerKey);
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
	(!isDuplicatable(fieldStatus) && recipe.m_Status & fieldStatus) ? throw BadRequestException() : 0;
	ABNF::compareOneCharacter(message, pos, BNF::E_RESERVED::COLON) ? 0 : throw BadRequestException();

	std::vector<std::string>	args;
	skipWSP(message, pos);
	fieldValue(message, pos, args, fieldStatus);
	skipWSP(message, pos);
	argumentChecker(recipe, args, fieldStatus);
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
	HTTP::RequestMessageParser::MethodMap_t::const_iterator	it = m_MethodMap.find(method);
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


/**
 *		Body를 처리하는 함수
 *		content-length만큼 body를 읽는다.

 *		문제
 *		1. content-length보다 body사이즈가 작을 때	=> 추가로 데이터가 들어올 때까지 대기
 *		2. content-length보다 body사이즈가 클 때	=> 뒤에 데이터 버림
 *		3. content-length가 없는데 body는 있는 경우 (POST 등) => 아직 모름
 *		4. chunekd로 들어온 경우 => chunekd 전용 함수 만들어서 처리 (status로 비교가능)
 
 *		클라이언트 request -> Parser() 호출
 *		이전에 만들어놓은 recipe가 존재하는지 -> 존재하면 body만 파싱 (chunked 또는 content-length가 body보다 큰 경우)
*/
void	HTTP::RequestMessageParser::messageBody(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	const std::map<unsigned short, void*>::const_iterator	it = recipe.m_HeaderMap.find(E_HTTP::E_HEADER::CONTENT_LENGTHS);
	if (it == recipe.m_HeaderMap.end()) {
		return ;
	}
}

/**
 *		Main Parser
*/
HTTP::ResponseRecipe	HTTP::RequestMessageParser::Parser(HTTP::ResponseRecipe& recipe, const std::string &message) {
	std::size_t				pos(0);
	bool					checkBit(false);

	// 만약 이전에 recipe를 만들었으면 (status로 비교)
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