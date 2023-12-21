#include "RequestMessageParser.hpp"
#include <cctype>
#include <string>

/**
 *		Parse Utils
*/
void	skipWSP(const std::string& message, std::size_t& pos) {
	const std::size_t&	messageSize(message.size());

	while (pos < messageSize && ABNF::isWSP(message, pos)) {
		pos++;
	}
}

void	token(const std::string& message, std::size_t& pos, std::string& argument) {
	const std::size_t&	messageSize(message.size());

	while (pos < messageSize && ABNF::isTCHAR(message, pos)) {
		argument += message[pos];
	}
}


/**
 *		Field Line (Header Part)
 *		Header의 내용들을 정의해둬서 argument, argumentChekcer와 같이 운용
*/
void	fieldValue(const std::string& message, std::size_t& pos) {

}

void	fieldLine(const std::string& message, std::size_t& pos, bool& checkBit) {
	const std::size_t	messageSize(message.size());
	if (ABNF::isCRLF(message, pos)) {
		return (checkBit) ? static_cast<void>(0) : throw ; 
	}
	checkBit |= true;

	std::string		fieldName;
	token(message, pos, fieldName);
	ABNF::compareOneCharacter<T>(message, pos, BNF::E_RESERVED::COLON);

	skipWSP(message, pos);
	fieldValue(message, pos);
	skipWSP(message, pos);
}



/**
 *		HTTP Parse
*/
void	HTTPVersion(const std::string& message, std::size_t& pos) {
	const std::size_t	messageSize(message.size());
	std::string			HTTPName;

	for (std::size_t i(0); i < 4 && pos < messageSize; ++i, ++pos) {
		HTTPName += message[pos];
	}
	(HTTPName == "HTTP") ? 0 : throw ;

	ABNF::compareOneCharacter<T>(message, pos, BNF::E_RESERVED::SLASH);

	// TODO: 맞지 않는 버전의 경우 어떻게 처리할 것인가?
	float HTTPVersion = (pos < messageSize && std::isdigit(static_cast<int>(message[pos]))) ? static_cast<float>(message[pos] - '0') : throw ;
	ABNF::compareOneCharacter<T>(message, ++pos, BNF::E_MARK::PERIOD);
	HTTPVersion += (pos < messageSize && std::isdigit(static_cast<int>(message[pos]))) ? (static_cast<float>(message[pos] - '0') / 10) : throw ;
	++pos;
	// TODO: 맞지 않는 버전에 대한 예외 처리
	// 만약 클라이언트가 1.1이 아니면 낮은 버전으로 처리해야 된다.
	(HTTPVersion > 1.1) ? ;
}


/**
 *		Method (GET / POST / DELETE ...)
 *		std::map에 key로 method를 갖고, value로 status(char)를 저장하여 비트마스킹
 *		GET / DELETE 이면 request-target을 저장할 필요 없이 바로 꺼내서 주면 됨
 *		POST의 경우 BODY의 내용까지 다 읽고 request-target에 실행해야 하므로 저장해야됨
*/
void	method(const std::string& message, std::size_t& pos) {
	std::string	method;

	token(message, pos, method);
	(m_Method.find(method) != m_Method.end()) status |= : throw ("501");
}



/**
 *		Start Line
*/
void	requesetLine(const std::string& message, std::size_t& pos) {
	method(message, pos);
	ABNF::compareOneCharacter<T>(message, pos, E_ABNF::SP);
	requestTarget(message, pos);
	ABNF::compareOneCharacter<T>(message, pos, E_ABNF::SP);
	HTTPVersion(message, pos);
}


/**
 *		Main Parser
*/
bool	HTTP::RequestMessageParser(const std::string& message) {
	std::size_t		pos(0);
	bool			checkBit(false);

	requestLine(message, pos);
	while (pos < message.length() && ABNF::isCRLF(message, pos)) {
		pos += 2;
		fieldLine(message, pos, checkBit);
	}
	messageBody(message, pos);
}
