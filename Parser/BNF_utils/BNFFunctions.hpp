#pragma once

#include <string>

namespace BNF {
	namespace E_RESERVED {
		enum E_RESERVED {
			SEMICOLON = ';',
			SLASH = '/',
			QUESTION_MARK = '?',
			COLON = ':',
			AT_SIGN = '@',
			AMPERSAND = '&',
			EQUALS = '=',
			PLUS = '+',
			DOLLAR_SIGN = '$',
			COMMA = ','
		};
	}

	namespace E_MARK {
		enum E_MARK {
			HYPHEN = '-',
			UNDERSCORE = '_',
			PERIOD = '.',
			EXCLAMATION_MARK = '!',
			TILDE = '~',
			ASTERISK = '*',
			SINGLE_QUOTE = '\'',
			LEFT_PARENTHESIS = '(',
			RIGHT_PARENTHESIS = ')'
		};
	}

	namespace E_PCHAR {
		enum E_PCHAR {
			COLON = ':',
			AT_SIGN = '@',
			AMPERSAND = '&',
			PLUS = '+',
			DOLLAR_SIGN = '$',
			COMMA = ','
		};
	}

	bool	isPcharReserved(const std::string& inputURI, std::size_t& pos);
	bool	isReserved(const std::string& inputURI, std::size_t& pos);
	bool	isMark(const std::string& inputURI, std::size_t& pos);
	bool	isUnreserved(const std::string& inputURI, std::size_t& pos);
	bool	isEscaped(const std::string& inputURI, std::size_t& pos);
	bool	isPchar(const std::string& inputURI, std::size_t& pos);
	bool	isUric(const std::string& inputURI, std::size_t& pos);
}