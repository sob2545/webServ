#include "BNFFunctions.hpp"

bool	BNF::isPcharReserved(const std::string& inputURI, std::size_t& pos) {
	switch (inputURI.at(pos)) {
		case (BNF::E_PCHAR::COLON):
		case (BNF::E_PCHAR::AT_SIGN):
		case (BNF::E_PCHAR::AMPERSAND):
		case (BNF::E_RESERVED::EQUALS):
		case (BNF::E_PCHAR::PLUS):
		case (BNF::E_PCHAR::DOLLAR_SIGN):
		case (BNF::E_PCHAR::COMMA):
			return true;
		default :
			return false;
	}
}

bool	BNF::isReserved(const std::string& inputURI, std::size_t& pos) {
	switch (inputURI.at(pos)) {
		case (BNF::E_RESERVED::SEMICOLON):
		case (BNF::E_RESERVED::SLASH):
		case (BNF::E_RESERVED::QUESTION_MARK):
		case (BNF::E_RESERVED::COLON):
		case (BNF::E_RESERVED::AT_SIGN):
		case (BNF::E_RESERVED::AMPERSAND):
		case (BNF::E_RESERVED::EQUALS):
		case (BNF::E_RESERVED::PLUS):
		case (BNF::E_RESERVED::DOLLAR_SIGN):
		case (BNF::E_RESERVED::COMMA):
			return true;
		default :
			return false;
	}
}

bool	BNF::isMark(const std::string& inputURI, std::size_t& pos) {
	switch (inputURI.at(pos)) {
		case (BNF::E_MARK::HYPHEN):
		case (BNF::E_MARK::UNDERSCORE):
		case (BNF::E_MARK::PERIOD):
		case (BNF::E_MARK::EXCLAMATION_MARK):
		case (BNF::E_MARK::TILDE):
		case (BNF::E_MARK::ASTERISK):
		case (BNF::E_MARK::SINGLE_QUOTE):
		case (BNF::E_MARK::LEFT_PARENTHESIS):
		case (BNF::E_MARK::RIGHT_PARENTHESIS):
			return true;
		default:
			return false;
	}
}

bool	BNF::isUnreserved(const std::string& inputURI, std::size_t& pos) {
	const unsigned char c = static_cast<unsigned char>(inputURI.at(pos));

	return ((std::isalnum(c) || isMark(inputURI, pos)) ? true : false);
}

bool	BNF::isEscaped(const std::string& inputURI, std::size_t& pos) {
	if (inputURI.at(pos) == '%') {
		if (pos + 2 < inputURI.size()) {
			return (isxdigit(inputURI[pos + 1]) && isxdigit(inputURI[pos + 2])) ? true : false;
		}
		pos += 2;
	}
	return false;
}

bool	BNF::isPchar(const std::string& inputURI, std::size_t& pos) {
	if (isUnreserved(inputURI, pos) || isEscaped(inputURI, pos) || isPcharReserved(inputURI, pos)) {
		return true;
	} else {
		return false;
	}
}

bool	BNF::isUric(const std::string& inputURI, std::size_t& pos) {
	return (BNF::isPchar(inputURI, pos)
			|| inputURI.at(pos) == BNF::E_RESERVED::SLASH
			|| inputURI.at(pos) == BNF::E_RESERVED::QUESTION_MARK);
}