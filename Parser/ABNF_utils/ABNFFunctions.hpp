#pragma once

#include <string>

namespace E_ABNF {
	enum E_SPECIAL_LETTERS{
		CR = '\r',
		LF = '\n',
		HTAB = '\t',
		SP = ' ',
		UNDERSCORE = '_',
		DQUOTE = '\"',
		SEMICOLON = ';'
	};
}

namespace E_HTTP {
	enum E_TCHAR {
		EXCLAMATION = '!',
		HASH = '#',
		DOLLAR = '$',
		PERCENT = '%',
		AMPERSAND = '&',
		SINGLE_QUOTE = '\'',
		ASTERISK = '*',
		PLUS = '+',
		MINUS = '-',
		DOT = '.',
		CARET = '^',
		BACK_QUOTE = '`',
		VERTICAL_BAR = '|',
		TILDE = '~'
	};
}

namespace ABNF {
    bool	isLF(const std::string& file, const std::size_t& pos);
    bool	isWSP(const std::string& file, const std::size_t& pos);
    bool	isComment(const std::string& file, std::size_t& pos);
    bool	isC_nl(const std::string& file, std::size_t& pos);
	bool	isCRLF(const std::string& file, const std::size_t& pos);
	bool	isTCHAR(const std::string& file, const std::size_t& pos);
	bool	compareOneCharacter(const std::string& inputURI, std::size_t& pos, const unsigned char& toCmp);
}