#include "ABNFFunctions.hpp"
#include <cctype>

bool	ABNF::isTCHAR(const std::string& file, const std::size_t& pos) {
	switch (file[pos]) {
		case (E_HTTP::EXCLAMATION):
		case (E_HTTP::HASH):
		case (E_HTTP::DOLLAR):
		case (E_HTTP::PERCENT):
		case (E_HTTP::AMPERSAND):
		case (E_HTTP::SINGLE_QUOTE):
		case (E_HTTP::ASTERISK):
		case (E_HTTP::PLUS):
		case (E_HTTP::MINUS):
		case (E_HTTP::DOT):
		case (E_HTTP::CARET):
		case (E_HTTP::BACK_QUOTE):
		case (E_HTTP::VERTICAL_BAR):
		case (E_HTTP::TILDE):
			return true;
	}
	return (std::isalnum(static_cast<int>(file[pos])));
}


bool	ABNF::isLF(const std::string& file, const std::size_t& pos) {
	return ((file[pos] == E_ABNF::LF) ? true : false);
}

bool	ABNF::isWSP(const std::string& file, const std::size_t& pos) {
	switch (file.at(pos)) {
		case (E_ABNF::HTAB):
		case (E_ABNF::SP):
			return true;
	}
	return false;
}

bool	ABNF::isComment(const std::string& file, std::size_t& pos) {
	if (file.at(pos) == E_ABNF::SEMICOLON) {
		pos++;
	} else {
		return false;
	}
	// isprint()만 사용하면 알파벳 이외의 한글을 읽지 못함
	while (pos < file.size() && file[pos] != E_ABNF::LF) {
		pos++;
	}
	return (isLF(file, pos));
}

bool	ABNF::isC_nl(const std::string& file, std::size_t& pos) {
	if (isComment(file, pos) || isLF(file, pos)) {
		pos++;
		return true;
	} else {
		return false;
	}
}

bool	ABNF::isCRLF(const std::string& file, const std::size_t& pos) {
	return ((pos < file.length()) && file[pos] == E_ABNF::CR && file[pos + 1] == E_ABNF::LF);
}

bool	ABNF::compareOneCharacter(const std::string& inputURI, std::size_t& pos, const unsigned char& toCmp) {
	const std::size_t&		URILength = inputURI.length();
	const std::string		src(&inputURI[pos]);

	if (pos >= URILength || inputURI[pos] != toCmp) {
		return false;
	}
	pos++;
	return true;
}