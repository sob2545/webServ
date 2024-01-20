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
	m_MethodMap["GET"] = E_HTTP::GET;
	m_MethodMap["POST"] = E_HTTP::POST;
	m_MethodMap["DELETE"] = E_HTTP::DELETE;
	m_MethodMap["PUT"] = E_HTTP::PUT;
	m_MethodMap["PATCH"] = E_HTTP::PATCH;
	m_MethodMap["OPTIONS"] = E_HTTP::OPTIONS;
}

void	HTTP::RequestMessageParser::initHeaderMap() {
	m_HeaderMap["Connection"] = E_HTTP::CONNECTION;
	m_HeaderMap["Date"] = E_HTTP::DATE;
	m_HeaderMap["Transfer-Encoding"] = E_HTTP::TRANSFER_ENCODING;
	m_HeaderMap["Host"] = E_HTTP::HOST;
	m_HeaderMap["Content-Lengths"] = E_HTTP::CONTENT_LENGTHS;
	m_HeaderMap["Content-Type"] = E_HTTP::CONTENT_TYPE;
	m_HeaderMap["Cookie"] = E_HTTP::COOKIE;
}

bool	HTTP::RequestMessageParser::isDuplicatable(const unsigned short& status) {
	switch (status) {
		case (E_HTTP::CONNECTION):
		case (E_HTTP::DATE):
		case (E_HTTP::TRANSFER_ENCODING):
		case (E_HTTP::COOKIE):
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
		case (E_HTTP::CONNECTION): {
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
		case (E_HTTP::DATE): {
			// DateParser();
		}
		case (E_HTTP::TRANSFER_ENCODING): {
			for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
				std::map<unsigned short, void*>::iterator encodingStatus = recipe.m_HeaderMap.find((status));

				if (encodingStatus != recipe.m_HeaderMap.end()) {
					(!encodingStatus->second) ? (*reinterpret_cast<bool*>(encodingStatus->second) = ((*it == "chunekd") ? true : false)) : 0;
				} else {
					recipe.m_HeaderMap.insert(std::make_pair(status, (static_cast<void*>(::new bool((*it == "chunked") ? true : false)))));
				}
			}
		}
		case (E_HTTP::HOST): {
			// argument길이가 최대 몇 자 넘어가면 exception
			recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new std::string(args[0]))));
		}
		case (E_HTTP::CONTENT_LENGTHS): {
			(args.size() != 1) ? throw BadRequestException() : 0;
			char*	endptr;
			const long		length = strtol(args[0].c_str(), &endptr, 10);
			(length <= 0) ? throw BadRequestException() : 0;
			recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new int(length))));
		}
		case (E_HTTP::CONTENT_TYPE): {
			(args.size() != 1) ? throw BadRequestException() : 0;
			MasterProcess::getMIMETypes().find(args[0]) != MasterProcess::getMIMETypes().end() ?
				recipe.m_HeaderMap.insert(std::make_pair(status, static_cast<void*>(::new std::string(args[0])))) : throw BadRequestException();
		}
		case (E_HTTP::COOKIE): {
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
	const char		delimiter = (status & E_HTTP::TRANSFER_ENCODING || status & E_HTTP::CONNECTION) ?
								E_HTTP::COMMA : ((status & E_HTTP::COOKIE) ?
																E_HTTP::SEMICOLON : E_HTTP::DEFAULT);

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
	if (status & E_HTTP::HOST) {
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

/*
 *		Chunekd Parser
*/
void	HTTP::RequestMessageParser::handleChunekdMsg(HTTP::ResponseRecipe& recipe, const std::string& message, std::size_t& pos) {
	const std::size_t&	messageSize(message.size());
	std::string			chunekdSize;

	/*
	 *	chunk-size를 contentLength로 갱신하여 저장할 필요가 있음
	 *	안 그러면 chunk-size보다 데이터가 작은 경우 다시 읽는 작업을 할 수 없음
	 *	추가로 Parser에 들어왔을 때 이미 chunked인 경우 위에 Header를 읽을 필요가 없기 때문에 status로 빠른 처리 필요
	*/
	while (pos < messageSize && std::isxdigit(static_cast<int>(message[pos]))) {
		chunekdSize += message[pos];
		pos++;
	}
	char*			endptr;
	const long		chunekdLength = strtol(chunekdSize.c_str(), &endptr, 16);

	chunekdLength < 0 ? throw BadRequestException() : 0;
	while (pos < messageSize && ABNF::isC_nl(message, pos)) {
		(ABNF::isWSP(message, pos) || ABNF::isTCHAR(message, pos)) ? pos++ : throw BadRequestException();
	}
	if (chunekdLength == 0) {
		// chunked가 끝났다는 의미
		return ;
	}
	for (std::size_t i(0); i < chunekdLength && pos < messageSize; ++i) {
		recipe.m_BodyMessage.push_back(static_cast<unsigned char>(message[pos++]));
	}
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
	const std::map<unsigned short, void*>::const_iterator	it = recipe.m_HeaderMap.find(E_HTTP::CONTENT_LENGTHS);
	if (it == recipe.m_HeaderMap.end()) {
		return ;
	}

	const int&		contentLength = *reinterpret_cast<int*>(it->second);
	if (recipe.m_Status & E_HTTP::TRANSFER_ENCODING) {
		/*
		 *	chunk-size보다 데이터가 작은 경우, 추가로 들어온 데이터를 읽어줘야됨
		 *	문제는 웹 브라우저가 아닌 telnet 등으로 보냈을 때, 마지막에 CRLF가 포함되어 있음
		 *	chunk-size까지만 읽으면 CRLF가 남아있기 때문에 다음 read에서 오류가 발생 (이 문제에 대해 논의 필요)
		*/
		if (contentLength) {
			for (std::size_t i(0); i < contentLength && pos < message.size(); ++i) {
				recipe.m_BodyMessage.push_back(static_cast<unsigned char>(message[pos++]));
			}
		}
		handleChunekdMsg(recipe, message, pos);
	} else {
		const int&		bodySize = message.size() - pos;
		for (int i(0); i < contentLength && i < bodySize; ++i) {
			recipe.m_BodyMessage.push_back(static_cast<unsigned char>(message[pos++]));
		}
		/*
			content-length가 bodySize보다 크면 추가로 데이터가 들어올 때까지 대기
			그렇지 않으면 클라이언트한테 write 후 새롭게 read
			따라서 struct에 쓸 준비가 되었는지 아직 읽어야 되는지 상태를 저장해야됨 (isEnd 등)
		*/
		if (contentLength > bodySize) {
			*reinterpret_cast<int*>(it->second) = contentLength - bodySize;
		}

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